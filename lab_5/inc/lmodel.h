#ifndef __MODEL_H__
#define __MODEL_H__

#include "lqueue.h"
#include "defines.h"


int l_queue_model(tm_res *res);
int l_add_rnd_elem(lqueue_t *q1, lqueue_t *q2, queue_time time);
int l_add_to_call(lqueue_t *q1, lqueue_t *q2, lqueue_t *coll);
int l_pop_from_coll(lqueue_t *coll, tm_res *res);

#endif // __MODEL_H__
