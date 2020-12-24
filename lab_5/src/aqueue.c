#include <stdarg.h>
#include <stdlib.h>
#include "logger.h"
#include "aqueue.h"
#include "errors.h"
#include "defines.h"

static inline void shift_left(data_t *arr, int len)
{
    for (int i = 1; i < len; i++)
        arr[i - 1] = arr[i];
}

aqueue_t *init_aqueue(void)
{
    aqueue_t *queue = calloc(1, sizeof(aqueue_t));

    if (queue == NULL)
        return NULL;
    
    queue->capacity = AQUEUE_INIT_LEN;
    queue->arr = malloc(sizeof(data_t) * AQUEUE_INIT_LEN);

    if (queue->arr == NULL)
    {
        free(queue);
        return NULL;
    }

    INFO_PRINT("Aqueue initted %p\n", GENERICK_POINTER(queue));

    return queue;
}

void full_clean_aqueue(aqueue_t **aqueue)
{
    if (*aqueue != NULL)
    {
        INFO_PRINT("Cleaning %p \t arr: %p\n", GENERICK_POINTER(*aqueue), GENERICK_POINTER((*aqueue)->arr));
        if ((*aqueue)->arr)
            free((*aqueue)->arr);

        free(*aqueue);
    }
}

int add_aqueue(aqueue_t *aqueue, const data_t data)
{
    if (aqueue->len >= aqueue->capacity)
    {
        aqueue->capacity *= 2;
        aqueue->arr = realloc(aqueue->arr, sizeof(data_t) * aqueue->capacity);
        TRACE_PRINT("Realloced\n");

        if (aqueue->arr == NULL)
        {
            ERROR_PRINT("Alloc failed\n");
            full_clean_aqueue(&aqueue);
            return ALLOC_ERROR;
        }
    }

    if (print_pointers)
        printf("[Добавление] [Массив] %p\n", (void *) aqueue->arr + aqueue->len);

    aqueue->arr[aqueue->len++] = data;

    return NO_ERROR;
}

int pop_aqueue(aqueue_t *aqueue, data_t *elem)
{
    if (aqueue->len == 0)
        return 1;
    
    *elem = aqueue->arr[0];

    if (print_pointers)
        printf("[  Удаление] [Массив] %p\n", (void *) aqueue->arr);

    shift_left(aqueue->arr, aqueue->len);
    aqueue->len--;

    return 0;
}

void print_aqueue(aqueue_t *aqueue, print_fun __print)
{
    for (int i = 0; i < aqueue->len; i++)
        __print(&aqueue->arr[i]);
}

void clean_aqueues(int len, ...)
{
    va_list args;

    va_start(args, len);

    while (len-- > 0)
    {
        aqueue_t **aqueue = va_arg(args, aqueue_t**);
        full_clean_aqueue(aqueue);
    }

    va_end(args);
}

void *find_aqueue(aqueue_t *aqueue, void *data, cmp_fun __cmp)
{
    for (int i = 0; i < aqueue->len; i++)
        if (__cmp(data, &(aqueue->arr[i])) == 0)
            return &aqueue->arr[i];
    return NULL;
}
