#include <stdio.h>
#include "graph.h"
#include "vector.h"
#include "colors.h"
#include "funcs.h"
#include "timer.h"

int main(int argc, char **argv)
{
    system("figlet -f slant 'Graphs'");
    graph_t *graph = NULL;
    vector_t *map = NULL;

    struct timespec start_time_s, end_time_s; 

    FILE *fi = fopen(argv[1], "r");

    if (fi == NULL)
    {
        printf(RED_COLOR "Ошибка, неправильный путь к файлу.\n" RESET_COLOR);
        return 1;
    }

    if (get_file_size(fi) == 0)
    {
        printf(RED_COLOR "Ошибка, пустой файл.\n" RESET_COLOR);
        return 1;
    }

    if (input_graph(&graph, &map, fi))
        return 1;

    START_MEASURING();
    for (int i = 0; i < map->len; i++)
        longest_path(graph, i, map);
    END_MEASURING();

    SHOW_TIME("Время поиска");
    printf(YELLOW_COLOR "Памяти потребовалось: " RESET_COLOR "%luБ\n", map->alloc_len * sizeof(int) + sizeof(graph_t) + (sizeof(node_t*)) * graph->V + sizeof(adjlist_node_t) * graph->V / 2);


    FILE *f = fopen("graphviz.txt", "w");

    graph_to_jpeg(graph, "Test_graph", f, map);

    fclose(f);
    fclose(fi);
    clean_graph(graph);

    return 0;
}
