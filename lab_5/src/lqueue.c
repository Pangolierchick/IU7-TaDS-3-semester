#include <stdlib.h>
#include <stdarg.h>
#include "logger.h"
#include "lqueue.h"
#include "defines.h"
#include "errors.h"

lqueue_t *init_lqueue(void)
{
    lqueue_t *lqueue = calloc(1, sizeof(lqueue_t));

    if (lqueue == NULL)
    {
        ERROR_PRINT("Allocation failed\n");
        return NULL;
    }

    lqueue->head = NULL;
    lqueue->tail = NULL;
    lqueue->len = 0;

    TRACE_PRINT("Queue %p inited\n", GENERICK_POINTER(lqueue));

    return lqueue;
}

void full_clean_lqueue(lqueue_t **lqueue)
{
    if (*lqueue)
    {
        TRACE_PRINT("Cleaning %p (%d) lqueue\n", GENERICK_POINTER(*lqueue), (*lqueue)->len);

        queue_node_t *curr = (*lqueue)->tail;

        while (curr)
        {
            queue_node_t *next = curr->next;

            free(curr->data);
            free(curr);

            curr = next;
        }

        free(*lqueue);

        *lqueue = NULL;
    }
}

int add_lqueue(lqueue_t *lqueue, void *data)
{
    queue_node_t *node = calloc(1, sizeof(queue_node_t));

    if (node == NULL)
    {
        ERROR_PRINT("Allocation failed\n");
        return ALLOC_ERROR;
    }

    node->data = data;

    if (lqueue->len == 0)
    {
        lqueue->head = node;
        lqueue->tail = node;
    }
    else
    {
        lqueue->tail->next = node;
        lqueue->tail = node;
    }

    if (print_pointers)
        printf("[Добавление] [Список] %p\n", (void *) node);

    lqueue->len++;


    TRACE_PRINT("Queue curr len: %d\n", lqueue->len);

    return NO_ERROR;
}

void *pop_lqueue(lqueue_t *lqueue)
{
    if (lqueue->len <= 0)
    {
        TRACE_PRINT("Queue underflow\n");
        return NULL;
    }

    void *data = NULL;

    if (lqueue->len == 1)
    {
        data = lqueue->head->data;

        free(lqueue->head);
        lqueue->head = NULL;
        lqueue->tail = NULL;
    }
    else
    {
        // 2 -> 3 -> 4 -> 5

        TRACE_PRINT("Cleaning %p\n", lqueue->head);

        if (print_pointers)
            printf("[  Удаление] [Список] %p\n", (void *) lqueue->head);

        data = lqueue->head->data;
        free(lqueue->head);
        lqueue->head = lqueue->head->next;
    }


    lqueue->len--;

    TRACE_PRINT("Queue curr len: %d\n", lqueue->len);

    return data;
}

void print_lqueue(lqueue_t *lqueue, print_fun __print)
{
    INFO_PRINT("Printing lqueue (%d): \n", lqueue->len);

    FOREACH(lqueue, i)
    {
        DBG_PRINT("%p\n", GENERICK_POINTER(i->data));
        __print(i->data);
    }

    printf("\n");
}


void clean_lqueue(lqueue_t **lqueue)
{
    if (*lqueue)
    {
        TRACE_PRINT("Cleaning %p (%d) lqueue\n", GENERICK_POINTER(*lqueue), (*lqueue)->len);

        queue_node_t *curr = (*lqueue)->tail;

        while (curr)
        {
            queue_node_t *next = curr->next;

            TRACE_PRINT("Cleaning %p\t%p\n", GENERICK_POINTER(curr), curr->data);
            free(curr);

            curr = next;
        }

        free(*lqueue);

        *lqueue = NULL;
    }
}

void clean_lqueues(int len, ...)
{
    va_list args;

    va_start(args, len);

    while (len-- > 0)
    {
        lqueue_t **lqueue = va_arg(args, lqueue_t**);
        full_clean_lqueue(lqueue);
    }

    va_end(args);
}

void *find_lqueue(lqueue_t *lqueue, void *data, cmp_fun __cmp)
{
    FOREACH(lqueue, i)
    {
        if (__cmp(data, i) == 0)
            return i;
    }

    return NULL;
}
