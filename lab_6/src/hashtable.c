#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "hashtable.h"
#include "errors.h"
#include "logger.h"

int max_collisions;

static int is_prime(int num)
{
    if (num < 2)
        return 0;

    if (num % 2 == 0)
        return 0;

    for (int i = 3; i <= sqrt(num); i += 2)
        if (num % i == 0)
            return 0;

    return 1;
}

unsigned int next_prime(unsigned int num)
{
    unsigned int new = num;

    while (is_prime(new) == 0)
        new++;

    return new;
}

int hash_simple(int val, int len)
{
    unsigned int new = val;

    return new % len;
}

int hash_complicated(int val, int len)
{
    int key = abs(val);
    int N = len;
    double A = 0.618033;
    int h = N * fmod(key * A , 1);
    


   /*
    int key = val;

    key += ~(key << 16);
    key ^= (key >> 5);
    key += (key << 3);
    key ^= (key >> 13);
    key += ~(key << 9);
    key ^= (key >> 17);
    */


    return h % len;
}

#define FIND_FREE_ELEM(ht, ind, i, val)                                       \
    do                                                                        \
    {                                                                         \
        while (ind < ht->len && ht->arr[ind].free && ht->arr[ind].val != val) \
        {                                                                     \
            TRACE_PRINT("Collision %2d\n", i);                                \
            ind++;                                                            \
            i++;                                                              \
        }                                                                     \
        if (ind == ht->len - 1)                                               \
            return HASH_END;                                                  \
    } while (0);

#define FIND_ELEM(ht, ind, key, i)                                                 \
    do                                                                             \
    {                                                                              \
        while (ind < ht->len && i++ >= 0 && ht->arr[ind].val != key)               \
        {                                                                          \
            TRACE_PRINT("Finding [%d:%d] = %d\n", ind, ht->len, ht->arr[ind].val); \
            ind++;                                                                 \
        }                                                                          \
        if (ind == ht->len - 1)                                                    \
            return CACHE_MISS;                                                     \
    } while (0);

hashtable_t *hash_init(unsigned int len)
{
    hashtable_t *ht = malloc(sizeof(hashtable_t));

    if (ht == NULL)
        return NULL;

    ht->arr = calloc(1, sizeof(hashelem_t) * len);
    if (ht->arr == NULL)
    {
        free(ht);
        return NULL;
    }

    ht->len = len;

    INFO_PRINT("Memory allocated: %zu\n", sizeof(hashelem_t) * len + sizeof(hashtable_t));

    return ht;
}

void hash_clean(hashtable_t **ht)
{
    if (*ht)
    {
        if ((*ht)->arr)
            free((*ht)->arr);

        free(*ht);
        *ht = NULL;
    }
}

int hash_insert(hashtable_t *ht, const int val, const hashtype_t type)
{
    int res = 0;

    int (*hash_func)(int, int) = NULL;

    switch (type)
    {
    case SIMPLE:
    {
        hash_func = hash_simple;
    }
    case COMPLICATED:
    {
        hash_func = hash_func == NULL ? hash_complicated : hash_simple;

        int ind = hash_func(val, ht->len);

        TRACE_PRINT("hash[%2d] = %d (%d)\n", ind, ht->arr[ind].val, val);

        if (ht->arr[ind].val == val)
        {
            ht->arr[ind].free = 1;
            return 0;
        }
        int coll = 0;

        FIND_FREE_ELEM(ht, ind, coll, val);

        ht->arr[ind].val = val;
        ht->arr[ind].free = 1;

        res = coll;
        break;
    }
    default:
        res = -1;
        break;
    }

    return res;
}

int hash_find(const hashtable_t *ht, const int key, const hashtype_t type)
{
    int res = 0;

    int (*hash_func)(int, int) = NULL;

    switch (type)
    {
    case SIMPLE:
    {
        hash_func = hash_simple;
    }
    case COMPLICATED:
    {
        hash_func = hash_func == NULL ? hash_complicated : hash_simple;

        int ind = hash_func(key, ht->len);

        int cmpr = 0;

        DBG_PRINT("hash[%d] = %d (%d)\n", ind, ht->arr[ind].val, key);

        if (ht->arr[ind].val == key && ht->arr[ind].free)
        {
            DBG_PRINT("Found\n");
            cmpr = 1;
            return cmpr;
        }
        else
        {
            DBG_PRINT("Collision. Trying to find somewhere forward\n");
            FIND_ELEM(ht, ind, key, cmpr);

            if (ht->arr[ind].val == key && ht->arr[ind].free)
            {
                return cmpr + 1;
            }
            else
                res = -1;
        }
        break;
    }
    default:
        res = -1;
        break;
    }

    return res;
}

int hash_remove(hashtable_t *ht, const int key, const hashtype_t type)
{
    int res = 0;

    int (*hash_func)(int, int) = NULL;

    switch (type)
    {
    case SIMPLE:
    {
        hash_func = hash_simple;
    }
    case COMPLICATED:
    {
        hash_func = hash_func == NULL ? hash_complicated : hash_simple;

        int ind = hash_func(key, ht->len);

        if (ht->arr[ind].val == key)
        {
            ht->arr[ind].free = 0;
            res = 1;
        }
        else
        {
            int i = 0;
            FIND_ELEM(ht, ind, key, i);

            if (ht->arr[ind].val == key)
            {
                ht->arr[ind].free = 0;
                res = i;
            }
            else
                res = CACHE_MISS;
        }
    }
    default:
        res = -1;
        break;
    }

    return res;
}

hashtable_t *hash_resize(hashtable_t *ht, unsigned int len)
{
    hashtable_t *new = malloc(sizeof(hashtable_t));

    if (new == NULL)
        return NULL;

    new->arr = malloc(sizeof(hashelem_t) * len);

    INFO_PRINT("Resizing table to: %zu B\n", sizeof(hashelem_t) * len + sizeof(hashtable_t));

    if (new->arr == NULL)
    {
        free(new);
        return NULL;
    }

    int min = len < ht->len ? len : ht->len;

    new->len = len;

    memcpy(new->arr, ht->arr, min * sizeof(hashelem_t));

    hash_clean(&ht);

    return new;
}

void hash_print(const hashtable_t *ht)
{
    printf("+-----------+------------------+\n");
    printf("|     ХЭШ   |      ДАННЫЕ      |\n");
    for (int i = 0; i < ht->len; i++)
    {
        if (ht->arr[i].free)
        {
            printf("+-----------+------------------+\n");
            printf("|%7d    |  %7d         |\n", i, ht->arr[i].val);
        }
    }
    printf("+-----------+------------------+\n");
}
