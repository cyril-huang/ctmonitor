#ifndef _CTM_H
#define _CTM_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <gtk/gtk.h>

#ifdef DEBUG
#define ctm_debug(FMT, ...) \
	g_printf("%s %d %s :" FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define ctm_debug(FMT, ...)
#endif

#ifndef DATADIR
#define DATADIR "."
#endif

#define VERSION			"0.0.1"
#define MS			* 1000
#define SAMPLING_MSECOND	60000
#define SLICE_MSECOND		500
#define SLICE_SECOND		0	
#define SAMPLING_RATE		(SAMPLING_MSECOND / (SLICE_SECOND * 1000 + SLICE_MSECOND))

#define CORE_SYSPATH_SIZE	60
#define CORE_LABEL_SIZE		15
#define CTM_MAX(x, y)		x - ((x - y) & -(x < y))
#define CTM_MIN(x, y) 		y + ((x - y) & -(x < y))
#define INT_ROUND(dividend, divisor) \
	(int)((dividend / divisor) + ((dividend % divisor) ? 1 : 0))

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

typedef struct ct_queue {
	int *data;		/* timeline data for the temperature graph */
	int head;		/* the head of queue */
	int tail;		/* the tail of queue */
	size_t size;		/* size of queue */
} ct_queue_t;

typedef struct ct_info {
	char path[CORE_SYSPATH_SIZE];/* coretemp.0/temp1_ */
	char label[CORE_LABEL_SIZE]; /* from coretemp.?/temp1_label */
	int max;		/* max temperature from temp1_max */
	int crit;		/* critical temperature from temp1_crit */
	int crit_alarm;		/* critical alarm from temp1_crit_alarm */
	struct ct_queue *ct;
} ct_info_t;

typedef struct system_ct_info {
	int max;		/* the maximum temperature of all coretemp */
	int crit;		/* the minimum critical temperature of all */
} system_ct_info_t;


/* ctm api */
int init_ctm();
struct ct_queue *ct_queue_init(ct_queue_t *q, size_t size);
void ct_queue_push(ct_queue_t *q, int data);
void ctm_about(GtkAction *action, gpointer data);
#endif /* _CTM_H */
