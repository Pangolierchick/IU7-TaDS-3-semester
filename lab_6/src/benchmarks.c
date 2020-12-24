#include <stdio.h>
#include "timer.h"
#include "tree.h"
#include "hashtable.h"
#include "fileio.h"

long tree_search_avg(tnode_t *tree, int *arr, int len)
{
    uint64_t time = tick();

    for (int i = 0; i < len; i++)
        tree_find(tree, arr[i]);

    time = tick() - time;

    return time / len;
}

long hash_search_avg(hashtable_t *table, int *arr, int len)
{
    uint64_t time = tick();

    for (int i = 0; i < len; i++)
    {
        hash_find(table, arr[i], SIMPLE);
    }

    time = tick() - time;

    return time / len;
}


long linsearch_avg(FILE *f, int *arr, int len)
{
    uint64_t time = tick();

    for (int i = 0; i < len; i++)
        find_from_file(f, arr[i]);

    time = tick() - time;

    return time / len;
}

