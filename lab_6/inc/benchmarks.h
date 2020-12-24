#ifndef __BENCHMARKS_H__
#define __BENCHMARKS_H__

#include <stdio.h>
#include "tree.h"
#include "hashtable.h"

long tree_search_avg(tnode_t *tree, int *arr, int len);
long hash_search_avg(hashtable_t *table, int *arr, int len);
long linsearch_avg(FILE *f, int *arr, int len);

#endif // __BENCHMARKS_H__
