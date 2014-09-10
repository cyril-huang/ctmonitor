#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <glob.h>

#include "ctm.h"

extern struct ct_info *ct_core;
extern struct system_ct_info system_ct;
extern int ncore;

#define CTDIR "/sys/devices/platform"

static void modprobe_coretemp()
{
	FILE *f;
	char modules[] = "/proc/modules";
	char line[256];
	int module_existed = 0;

	f = fopen(modules, "r");
	if (f == NULL) {
		perror(modules);
		exit(errno);
	}

	while ((fgets(line, sizeof line, f)) != NULL) {
		if (!strstr(line, "coretemp")) {
			module_existed = 1;
			break;
		}
	}
	fclose(f);
	if (module_existed) {
		return;
	} else {
		/* FIXME: popup dialog for root password and load coretemp */
		fprintf(stderr, "No coretemp kernel module installed\n");
		exit(ENOENT);
	}
}

enum sysfile_type { SYSFILE_STRING, SYSFILE_INTEGER };

static int read_sysfile(char *path, void *buf, int content_type)
{
	FILE *f;
	int v;
	char *n;

	f = fopen(path, "r");
	if (f == NULL) {
		perror(path);
		return -1;
	}
	switch (content_type) {
		case SYSFILE_STRING:
			fgets((char *)buf, CORE_LABEL_SIZE, f);
			if ((n = strchr(buf, '\n')))
				*n = '\0';
			break;
		case SYSFILE_INTEGER:
			fscanf(f, "%d", &v);
			memcpy(buf, &v, sizeof(int));
			break;
		default:
			ctm_debug("NEVER be here %d\n", content_type);
	}
	fclose(f);
	return 0;
}

static int get_ctinfo()
{
	glob_t gbuf;
	int i, ret;

	ret = glob(CTDIR"/coretemp*/temp*_input", GLOB_ERR, NULL, &gbuf);

	if (ret == GLOB_NOMATCH) {
		perror("glob");
		ncore = 0;
		goto ctinfo_err;
	} else {
		ncore = gbuf.gl_pathc;
		ctm_debug("ncore %d \n", ncore);
		ct_core = (struct ct_info *)malloc(sizeof(struct ct_info) * ncore);
		for (i = 0; i < ncore; i++) {
			char *s = gbuf.gl_pathv[i];
			char *t = ct_core[i].path;

			while (*t++ = *s++);
			t -= 6;
			memcpy(t, "label", 6);
			read_sysfile(ct_core[i].path, ct_core[i].label,
					SYSFILE_STRING);
			memcpy(t, "max", 4);
			read_sysfile(ct_core[i].path, &ct_core[i].max,
					SYSFILE_INTEGER);
			memcpy(t, "crit", 5);
			read_sysfile(ct_core[i].path, &ct_core[i].crit,
					SYSFILE_INTEGER);
			memcpy(t, "crit_alarm", 11);
			read_sysfile(ct_core[i].path, &ct_core[i].crit_alarm,
					SYSFILE_INTEGER);
			system_ct.max = CTM_MAX(system_ct.max, ct_core[i].max);
			system_ct.crit = CTM_MIN(system_ct.crit, ct_core[i].crit);
			memcpy(t, "input", 6);
		}
	}
ctinfo_err:
	globfree(&gbuf);
	return ncore;
}

static int readct(int *core_id)
{
	int ret, temp;
	struct timespec slice = { SLICE_SECOND, SLICE_MSECOND * 1000000L};

	for (;;) {
		ctm_debug("keep reading %d\n", *core_id);
		ret = read_sysfile(ct_core[*core_id].path, &temp,
				   SYSFILE_INTEGER);
		if (ret < 0) {
			fprintf(stderr, "ctm [ERROR] Read %s error\n",
				ct_core[*core_id].path);
		} else {
			ct_queue_push(ct_core[*core_id].ct, temp);
		}
		nanosleep(&slice, NULL);
	}
}

static int *arg;
static pthread_t *tid;

int init_ctm()
{
	int i;

	modprobe_coretemp();
	ncore = get_ctinfo();
	if (!ncore) {
		fprintf(stderr, "No coretemp existed, new Intel CPU required\n");
		exit(ENOENT);
	}

	tid = (pthread_t *) malloc(sizeof(pthread_t) * ncore);
	arg = (int *) malloc(sizeof(int) * ncore);

	for (i = 0; i < ncore; i++) {
		arg[i] = i;
		ct_core[i].ct = ct_queue_init(ct_core[i].ct, SAMPLING_RATE);
		if (!ct_core[i].ct) {
			fprintf(stderr, "data queue creation failed\n");
			exit(ENOMEM);
		}
		ctm_debug("creating thread %d ...\n", i);
		pthread_create(&tid[i], NULL, (void *)readct, (void *)&arg[i]);
	}

	return ncore;
}
