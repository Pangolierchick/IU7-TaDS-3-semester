#ifndef __AQUEUE_H__
#define __AQUEUE_H__

#include <stdarg.h>
#include <stdlib.h>

#include "defines.h"

#define AQUEUE_INIT_LEN                    110

typedef void print_fun(void *);
typedef int cmp_fun(void *, void *);

// typedef double queue_time;

typedef struct aqueue_s
{
    int len;
    int capacity;

    data_t *arr;

    int was;
} aqueue_t;

aqueue_t *init_aqueue(void);
void full_clean_aqueue(aqueue_t **aqueue);

int add_aqueue(aqueue_t *aqueue, const data_t data);
int pop_aqueue(aqueue_t *aqueue, data_t *elem);

void print_aqueue(aqueue_t *aqueue, print_fun __print);

void clean_aqueues(int, ...);
void *find_aqueue(aqueue_t *aqueue, void *data, cmp_fun __cmp);

#endif // __AQUEUE_H__
