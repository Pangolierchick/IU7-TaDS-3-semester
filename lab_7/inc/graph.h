#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdbool.h>
#include "list.h"
#include "array_stack.h"
#include "vector.h"

typedef struct adjlist_node
{
    int v;
    int weight;
} adjlist_node_t;

typedef struct graph
{
    int V;
    int capV;
    node_t **adj;
} graph_t;

adjlist_node_t *init_adjlist_node(int v, int weight);

graph_t *init_graph(int V);
void clean_graph(graph_t *graph);

void graph_add_edge(graph_t *graph, int u, int v, int weight);
void topological_sort(graph_t *graph, int v, bool *visited, arr_stack_t *stack);
void longest_path(graph_t *graph, int s, vector_t *map);

void graph_to_jpeg(const graph_t *graph, const char *name, FILE *f, vector_t *map);

int input_graph(graph_t **__graph, vector_t **__map, FILE *f);

#endif // __GRAPH_H__
