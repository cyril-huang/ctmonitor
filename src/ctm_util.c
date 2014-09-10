#include <stdlib.h>
#include <stdio.h>
#include "ctm.h"

struct ct_queue *ct_queue_init(ct_queue_t *q, size_t size)
{
	q = (struct ct_queue *)malloc(sizeof(struct ct_queue));

	if (q == NULL) {
		perror("queue malloc");
	} else {
		q->data = malloc(sizeof(int) * size);
		q->size = size;
		q->head = q->tail = size - 1;
		q->data[q->head] = 0;
	}
	ctm_debug("ct queue size %d head %d tail %d\n", q->size, q->head, q->tail);

	return q;
}

void ct_queue_push(ct_queue_t *q, int data)
{
	if (unlikely(q->data[q->head] == 0)) {
		q->data[q->head] = data;
		return;
	}
	if (likely(q->head < q->tail)) {
		q->head = (q->head == 0) ? q->size - 1 : q->head - 1;
		q->data[--q->tail] = data;
	} else {
		if (q->tail != 0) {
			q->data[--q->tail] = data;
		} else {
			q->head--;
			q->tail = q->size - 1;
			q->data[q->tail] = data;
		}
	}
}
