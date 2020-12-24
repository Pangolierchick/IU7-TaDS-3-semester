#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "logger.h"
#include "errors.h"
#include "colors.h"
#include "tree.h"
#include "fileio.h"
#include "timer.h"
#include "hashtable.h"
#include "benchmarks.h"

int max_collisions = 0;

int main(int argc, char **argv)
{
    FILE *f = fopen(argv[1], "r");

    if (f == NULL)
    {
        printf(RED_COLOR "Ошибка, " RESET_COLOR "неправильное имя файла.\n");
        return BAD_FILE;
    }

    int *farr;
    int flen;

    if (read_from_file(f, &farr, &flen) == BAD_FILE || flen == 0)
    {
        printf(RED_COLOR "Ошибка " RESET_COLOR "чтения из файла. Проверьте, что файл содержит только целые числа.\n");
        return BAD_FILE;
    }

    uint64_t time;

    tnode_t *tree = NULL;

    time = tick();
    for (int i = 0; i < flen; i++)
    {
        if (tree_find(tree, farr[i]) == NULL)
            tree = tree_insert(tree, farr[i]);
    }    
    time = tick() - time;

    printf(YELLOW_COLOR "БИНАРНОЕ ЦЕЛОЧИСЛЕННОЕ ДЕРЕВО\n\n" RESET_COLOR);
    draw_tree_hor(tree);
    printf(GREEN_COLOR "\nДерево построенно за " RESET_COLOR YELLOW_COLOR "%llu" RESET_COLOR GREEN_COLOR " тактов.\n" RESET_COLOR, time);

    tnode_t *btree = NULL;

    time = tick();
    for (int i = 0; i < flen; i++)
    {
        if (tree_find(btree, farr[i]) == NULL)
            btree = tree_insert_b(btree, farr[i]);
    }
    time = tick() - time;

    printf(YELLOW_COLOR "БИНАРНОЕ СБАЛАНСИРОВАННОЕ ЦЕЛОЧИСЛЕННОЕ ДЕРЕВО\n\n" RESET_COLOR);
    draw_tree_hor(btree);
    printf(GREEN_COLOR "\nДерево сбалансировано за " RESET_COLOR YELLOW_COLOR "%llu" RESET_COLOR GREEN_COLOR " тактов.\n" RESET_COLOR, time);


    unsigned int table_size = next_prime(flen);
    hashtable_t *table = hash_init(table_size);

    time = tick();
    hash_simple(1234, table_size);
    time = tick() - time;
    
    printf(GREEN_COLOR "\nГенерация простой хеш-функции " RESET_COLOR YELLOW_COLOR "%llu" RESET_COLOR GREEN_COLOR " тактов.\n" RESET_COLOR, time);


    time = tick();
    hash_complicated(1234, table_size);
    time = tick() - time;
    
    printf(GREEN_COLOR "\nГенерация сложной хеш-функции " RESET_COLOR YELLOW_COLOR "%llu" RESET_COLOR GREEN_COLOR " тактов.\n" RESET_COLOR, time);

    int coll = 0;

    time = tick();
    for (int i = 0; i < flen; i++)
    {
        int tmp_coll = hash_insert(table, farr[i], SIMPLE);
        coll = coll > tmp_coll ? coll : tmp_coll;
    }

    time = tick() - time;

    printf(YELLOW_COLOR "\nХЭШ-ТАБЛИЦА НА ПРОСТОЙ ФУНКЦИИ\n\n" RESET_COLOR);
    
    hash_print(table);

    printf(MAGENTA_COLOR "Максмальное кол-во коллизий %d\n" RESET_COLOR, coll);
    printf(GREEN_COLOR "\nХэш-таблица построена за " RESET_COLOR YELLOW_COLOR "%llu" RESET_COLOR GREEN_COLOR " тактов.\n" RESET_COLOR, time);

    int user_coll;

    printf("\nВведите допустимое кол-во коллизий\n");
    if (scanf("%d", &user_coll) != 1 || user_coll < 0)
    {
        printf(RED_COLOR "Ошибка, " RESET_COLOR "введено неправильное кол-во коллизий.\n");
        free(farr);
        hash_clean(&table);
        tree_clean(tree);
        tree_clean(btree);
        fclose(f);
        return BAD_INPUT;
    }

    int type = SIMPLE;

    if (coll > user_coll)
    {
        type = SIMPLE;
        while (coll > user_coll)
        {
            DBG_PRINT("Trying again\n");
            hash_clean(&table);
            table_size = next_prime(table_size + 1);
            table = hash_init(table_size);

            coll = 0;

            time = tick();
            for (int i = 0; i < flen; i++)
            {
                int tmp_coll = hash_insert(table, farr[i], SIMPLE);
                coll = coll > tmp_coll ? coll : tmp_coll;
            }

            time = tick() - time;

            DBG_PRINT("collisions %d\n", coll);
        }

            printf(YELLOW_COLOR "\nХЭШ-ТАБЛИЦА НА СЛОЖНОЙ ФУНКЦИИ\n\n" RESET_COLOR);
    
            hash_print(table);

            printf(MAGENTA_COLOR "Максмальное кол-во коллизий %d\n" RESET_COLOR, coll);
            printf(GREEN_COLOR "\nХэш-таблица построена за " RESET_COLOR YELLOW_COLOR "%llu" RESET_COLOR GREEN_COLOR " тактов.\n" RESET_COLOR, time);
            printf("Длина таблицы: %d\n", table->len);

    }
    else
    {
        printf(GREEN_COLOR "Перестройка хэш-таблицы не требуется\n" RESET_COLOR);
    }

    int to_find;

    printf("Введите число, которое требуется найти: ");

    if (scanf("%d", &to_find) != 1)
    {
        printf(RED_COLOR "Ошибка, " RESET_COLOR "требуется ввести число.\n");
        free(farr);
        hash_clean(&table);
        tree_clean(tree);
        tree_clean(btree);
        fclose(f);
        return BAD_INPUT;
    }

    int vert = 0;
    int cmps = 0;

    time = tick();
    int comp = tree_find_cmps(tree, to_find);
    time = tick() - time;

    tree_cmprs(tree, &vert, &cmps, 0);

    if (comp > 0)
    {
        printf(YELLOW_COLOR "\nПОИСК В БИНАРНОМ ДЕРЕВЕ\n\n" RESET_COLOR);

        printf(GREEN_COLOR "\nЧисло '%d' найдено за  " RESET_COLOR YELLOW_COLOR "%llu" RESET_COLOR GREEN_COLOR " тактов.\n" RESET_COLOR, to_find, time);
        printf(MAGENTA_COLOR "Среднее время поиска %ld в тактах (по всем элементам из файла)\n" RESET_COLOR, tree_search_avg(tree, farr, flen));
        printf(MAGENTA_COLOR "Бинарное дерево занимает %lu B\n" RESET_COLOR, sizeof(tnode_t) * flen);
        printf(MAGENTA_COLOR "Кол-во сравнений %d\n" RESET_COLOR, comp);
        printf(MAGENTA_COLOR "Среднее кол-во сравнений %lf\n" RESET_COLOR, (double) cmps / vert);
    }
    else
    {
        printf(RED_COLOR "Данное число не было найдено.\n" RESET_COLOR);
        free(farr);
        hash_clean(&table);
        tree_clean(tree);
        tree_clean(btree);
        fclose(f);
        return BAD_INPUT;
    }


    vert = 0;
    cmps = 0;


    time = tick();
    comp = tree_find_cmps(btree, to_find);
    time = tick() - time;

    tree_cmprs(btree, &vert, &cmps, 0);

    if (comp > 0)
    {
        printf(YELLOW_COLOR "\nПОИСК В СБАЛАНСИРОВАННОМ БИНАРНОМ ДЕРЕВЕ\n\n" RESET_COLOR);

        printf(GREEN_COLOR "\nЧисло '%d' найдено за  " RESET_COLOR YELLOW_COLOR "%llu" RESET_COLOR GREEN_COLOR " тактов.\n" RESET_COLOR, to_find, time);
        printf(MAGENTA_COLOR "Среднее время поиска %ld в тактах (по всем элементам из файла)\n" RESET_COLOR, tree_search_avg(btree, farr, flen));
        printf(MAGENTA_COLOR "Сбалансированное бинарное дерево занимает %lu B\n" RESET_COLOR, sizeof(tnode_t) * flen);
        printf(MAGENTA_COLOR "Кол-во сравнений %d\n" RESET_COLOR, comp);
        printf(MAGENTA_COLOR "Среднее кол-во сравнений %lf\n" RESET_COLOR, (double) cmps / vert);
    }
    else
    {
        printf(RED_COLOR "Данное число не было найдено.\n" RESET_COLOR);
    }

    uint64_t time_2;
    time = tick();
    comp = hash_find(table, to_find, type);
    time_2 = tick();
    // time = tick() - time;

    if (comp > 0)
    {
        printf(YELLOW_COLOR "\nПОИСК В ХЭШ-ТАБЛИЦЕ\n\n" RESET_COLOR);

        printf(GREEN_COLOR "\nЧисло '%d' найдено за  " RESET_COLOR YELLOW_COLOR "%llu" RESET_COLOR GREEN_COLOR " тактов.\n" RESET_COLOR, to_find, time_2 - time);
        printf(MAGENTA_COLOR "Среднее время поиска %ld в тактах (по всем элементам из файла)\n" RESET_COLOR, hash_search_avg(table, farr, flen));
        printf(MAGENTA_COLOR "Хэш-таблица занимает %lu B\n" RESET_COLOR, sizeof(hashelem_t) * table->len + sizeof(hashtable_t));
        printf(MAGENTA_COLOR "Кол-во сравнений %d\n" RESET_COLOR, comp);
        printf(MAGENTA_COLOR "Среднее кол-во сравнений %lf\n" RESET_COLOR, (double) (1 + user_coll) / 2);
    }
    else
    {
        printf(RED_COLOR "Данное число не было найдено.\n" RESET_COLOR);
    }

    time = tick();
    comp = find_from_file(f, to_find);
    time = tick() - time;

    if (comp > 0)
    {
        printf(YELLOW_COLOR "\nПОИСК В ФАЙЛЕ\n\n" RESET_COLOR);

        printf(GREEN_COLOR "\nЧисло '%d' найдено за  " RESET_COLOR YELLOW_COLOR "%llu" RESET_COLOR GREEN_COLOR " тактов.\n" RESET_COLOR, to_find, time);
        printf(MAGENTA_COLOR "Среднее время поиска %ld в тактах (по всем элементам из файла)\n" RESET_COLOR, linsearch_avg(f, farr, flen));
        printf(MAGENTA_COLOR "Файл занимает %d B\n" RESET_COLOR, get_file_size(f));
        printf(MAGENTA_COLOR "Кол-во сравнений %d\n" RESET_COLOR, comp);
        printf(MAGENTA_COLOR "Среднее кол-во сравнений %lf\n" RESET_COLOR, (double) flen / 2);
    }
    else
    {
        printf(RED_COLOR "Данное число не было найдено в файле.\n" RESET_COLOR);

    }

    free(farr);
    hash_clean(&table);
    tree_clean(tree);
    tree_clean(btree);
    fclose(f);

    return 0;
}
