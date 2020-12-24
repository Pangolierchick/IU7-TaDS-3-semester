#ifndef __AMODEL_H__
#define __AMODEL_H__

#include "aqueue.h"
#include "defines.h"

int a_queue_model(tm_res *res);
int a_add_rnd_elem(aqueue_t *q1, aqueue_t *q2, queue_time time);
int a_add_to_call(aqueue_t *q1, aqueue_t *q2, aqueue_t *coll);
int a_pop_from_coll(aqueue_t *coll, tm_res *res);

#endif // __AMODEL_H__
