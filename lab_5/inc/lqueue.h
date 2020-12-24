#ifndef __LQUEUE_H__
#define __LQUEUE_H__

#include <stdarg.h>
#include <stdlib.h>

#define FOREACH(__list, __i) for (queue_node_t *__i = __list->head; __i != NULL; __i = __i->next)

typedef void print_fun(void *);
typedef int cmp_fun(void *, void *);

struct queue_node_s
{
    void *data;
    struct queue_node_s *next;
};

typedef struct queue_node_s queue_node_t;

typedef struct queue_s
{
    int len;
    queue_node_t *head;
    queue_node_t *tail;

    int was;
} lqueue_t;

lqueue_t *init_lqueue(void);
void full_clean_lqueue(lqueue_t **lqueue);
void clean_lqueue(lqueue_t **lqueue);

int add_lqueue(lqueue_t *lqueue, void *data); // add new tail
void *pop_lqueue(lqueue_t *lqueue);           // remove head

void print_lqueue(lqueue_t *lqueue, print_fun __print);

void clean_lqueues(int, ...);
void *find_lqueue(lqueue_t *lqueue, void *data, cmp_fun __cmp);

#endif // __LQUEUE_H__
