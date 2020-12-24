#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "graph.h"
#include "colors.h"

#define NINF            INT_MIN

adjlist_node_t *init_adjlist_node(int v, int weight)
{
    adjlist_node_t *node = malloc(sizeof(adjlist_node_t));

    if (node)
    {
        node->v = v;
        node->weight = weight;
    }

    return node;
}

graph_t *init_graph(int V)
{
    graph_t *graph = malloc(sizeof(graph_t));

    if (graph)
    {
        graph->capV = V;
        graph->V = 0;
        graph->adj = calloc(V, sizeof(node_t *));

        if (graph->adj == NULL)
        {
            free(graph);
            return NULL;
        }
    }   

    return graph;
}

void clean_graph(graph_t *graph)
{
    if (graph)
    {
        for (int i = 0; i < graph->V; i++)
        {
            full_clear_list(graph->adj + i);
        }

        free(graph->adj);
        free(graph);
    }
}

void graph_add_edge(graph_t *graph, int u, int v, int weight)
{
    adjlist_node_t *node = init_adjlist_node(v, weight);

    push(&graph->adj[u], node);
}

void topological_sort(graph_t *graph, int v, bool *visited, arr_stack_t *stack)
{
    visited[v] = true;

    FOREACH(graph->adj[v], i)
    {
        if (!visited[((adjlist_node_t*) i->data)->v])
            topological_sort(graph, ((adjlist_node_t*) i->data)->v, visited, stack);
    }

    arr_append(stack, v);
}

void longest_path(graph_t *graph, int s, vector_t *map)
{
    arr_stack_t *stack = arr_init_stack();

    if (stack == NULL)
    {
        return;
    }

    int *dist = calloc(graph->V, sizeof(int));

    if (dist == NULL)
    {
        arr_clear_stack(&stack);
        return;
    }

    bool *visited = calloc(graph->V, sizeof(bool));

    if (visited == NULL)
    {
        arr_clear_stack(&stack);
        free(dist);
        return;
    }

    for (int i = 0; i < graph->V; i++)
        if (!visited[i])
            topological_sort(graph, i, visited, stack);
    
    for (int i = 0; i < graph->V; i++)
        dist[i] = NINF;
    dist[s] = 0;

    while (stack->len != 0)
    {
        int u = arr_pop(stack);

        if (dist[u] != NINF)
        {
            FOREACH(graph->adj[u], i)
            {
                if (dist[((adjlist_node_t*) i->data)->v] < dist[u] + ((adjlist_node_t*) i->data)->weight)
                    dist[((adjlist_node_t*) i->data)->v] = dist[u] + ((adjlist_node_t*) i->data)->weight;
            }
        }
    }

    printf(MAGENTA_COLOR "ДЛИННЕЙШИЕ ПУТИ ИЗ ВЕРШИНЫ %d" RESET_COLOR "\n", map->vec[s]);
    for (int i = 0; i < graph->V; i++)
    {
        if (dist[i] != NINF)
            printf("%d ==> %d [%3d]\n", map->vec[s], map->vec[i], dist[i]);
    }
    printf("\n");

    free(visited);
    arr_clear_stack(&stack);
    free(dist);
}

void graph_to_jpeg(const graph_t *graph, const char *name, FILE *f, vector_t *map)
{
    fprintf(f, "digraph %s {\n", name);
    for (int i = 0; i < graph->V; i++)
    {
        FOREACH(graph->adj[i], j)
        {
            fprintf(f, "%d -> %d [label=%d];\n", map->vec[i], map->vec[((adjlist_node_t*) j->data)->v], ((adjlist_node_t*) j->data)->weight);
        }
    }

    fprintf(f, "}\n");
}

int input_graph(graph_t **__graph, vector_t **__map, FILE *f)
{
    int vcount;

    if (fscanf(f, "%d", &vcount) != 1 || vcount <= 0)
    {
        printf(RED_COLOR "Ошибка ввода.\n" RESET_COLOR);
        return -1;
    }

    vector_t *map = init_vector();
    graph_t *graph = init_graph(vcount);

    if (map == NULL || graph == NULL)
    {
        clean_graph(graph);
        clean_vector(map);
        printf(RED_COLOR "Ошибка, не удалось выделить память.\n" RESET_COLOR);
        return 1;
    }

    while (1)
    {
        int u, v, w;
        int res = fscanf(f, "%d %d %d", &u, &v, &w);

        if (res == EOF)
            break;

        if (res != 3)
        {
            printf(RED_COLOR "Ошибка ввода.\n" RESET_COLOR);
            clean_graph(graph);
            clean_vector(map);
            return -1;
        }

        if (w == 0)
        {
            printf(RED_COLOR "Ошибка ввода. Метка дуги не может равняться нулю.\n" RESET_COLOR);
            clean_graph(graph);
            clean_vector(map);
            return -1;
        }

        if (u == -1)
            break;

        int ind = vec_find(map, &u);
        if (ind == -1)
        {
            if (map->len >= vcount)
            {
                printf(RED_COLOR "Ошибка ввода. Введенно больше вершин, чем заявлено\n" RESET_COLOR);
                clean_graph(graph);
                clean_vector(map);
                return -1;
            }

            ind = map->len;
            vec_append(map, &u);
        }

        int vind = vec_find(map, &v);
        if (vind == -1)
        {
            if (map->len >= vcount)
            {
                printf(RED_COLOR "Ошибка ввода. Введенно больше вершин, чем заявлено\n" RESET_COLOR);
                clean_graph(graph);
                clean_vector(map);
                return -1;
            }

            vind = map->len;
            vec_append(map, &v);
        }

        graph_add_edge(graph, ind, vind, w);
    }

    graph->V = map->len;
    *__graph = graph;
    *__map = map;

    return 0;
}
