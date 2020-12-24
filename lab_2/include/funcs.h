#ifndef __FUNCS_H__
#define __FUNCS_H__

#include <stdlib.h>
#include "static_vector.h"

#define BUBBLE_SORT         1
#define QUICK_SORT          2

#define START_MEASURING() gettimeofday(&start_time_s, NULL);
#define END_MEASURING() gettimeofday(&end_time_s, NULL);
#define SHOW_TIME() printf ("\x1b[33mОтсортировано. Время сортировки:\033[0m %lfus\n",\
         (double) (end_time_s.tv_usec - start_time_s.tv_usec) / 1000000 +\
         (double) (end_time_s.tv_sec - start_time_s.tv_sec));

#define SWAP(a, b, size)             \
    do                               \
    {                                \
        size_t __size = (size);      \
        char *__a = (a), *__b = (b); \
        do                           \
        {                            \
            char __tmp = *__a;       \
            *__a++ = *__b;           \
            *__b++ = __tmp;          \
        } while (--__size > 0);      \
    } while (0)                      \

typedef struct key_s
{
    int ind;
    int key;
} key_t;


typedef int cmp_fun(const void*, const void*);

int init_sort(static_vector_t *table, int type, int is_key);

void bubsort(void *base, size_t __nel, size_t __width, cmp_fun cmp);

int cost_cmp(const void *a, const void *b);
int cost_cmp_key(const void *a, const void *b);

int find_task(const static_vector_t *table);

#endif // __FUNCS_H__