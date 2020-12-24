#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include "funcs.h"
#include "error.h"
#include "defines.h"
#include "io.h"
#include "static_vector.h"

int cost_cmp_key(const void *a, const void *b)
{
    const key_t *pa = a;
    const key_t *pb = b;

    return (pa->key - pb->key);
}

int cost_cmp(const void *a, const void *b)
{
    const vector_type *pa = a;
    const vector_type *pb = b;

    return (pa->cost - pb->cost);
}

void bubsort(void *base, size_t __nel, size_t __width, cmp_fun cmp)
{
    char *base_ptr = (char *) base;

    for (size_t i = 0; i < __nel - 1; i++)
    {
        for (size_t j = 0; j < __nel - i - 1; j++)
        {
            if (cmp((void *) (base_ptr + (__width * j)), (void *) (base_ptr + (__width * (j + 1)))) > 0)
                SWAP((base_ptr + (__width * j)), (base_ptr + (__width * (j + 1))), __width);
        }
    }
}

int find_task(const static_vector_t *table)
{
    if (!table->len)
    {
        puts("Таблица пуста.");
        return NO_ERROR;
    }

    char find_table[BRAND_LEN + 2];
    int left_cost;
    int right_cost;

    printf("Введите бренд автомобиля: ");

    if (scanf("%20s", find_table) != 1)
        return BAD_BRAND;
    
    if (strlen(find_table) > BRAND_LEN)
        return BAD_BRAND;

    
    printf("Введите левый предел цены: ");
    if (scanf("%d", &left_cost) != 1)
        return BAD_COST;
    
    printf("Введите правый предел цены: ");
    if (scanf("%d", &right_cost) != 1)
        return BAD_COST;
    
    if (left_cost < 0 || right_cost < 0)
        return BAD_COST;

    int find_len = 0;

    for (int i = 0; i < table->len; i++)
    {
        if (table->vec[i].is_new == NO && table->vec[i].condition.old_auto.owner_num == 1 && !table->vec[i].condition.old_auto.repair_num)
        {
            if (table->vec[i].cost > left_cost && table->vec[i].cost < right_cost)
            {
                if (!strcmp(find_table, table->vec[i].brand))
                {
                    find_len++;
                    printf("# %d\t%d\n", i + 1, table->vec[i].cost);
                }
            }
        }
    }

    if (!find_len)
        puts("Не найдено ни одной записи.");
    
    return NO_ERROR;
}

static void key_cpy(static_vector_t *table, key_t *key_arr)
{
    for (int i = 0; i < table->len; i++)
    {
        key_arr[i].ind = i;
        key_arr[i].key = table->vec[i].cost;
    }
}

static void key_to_table_cpy(static_vector_t *table, key_t *key_arr)
{
    static_vector_t tmp;
    memcpy(&tmp, table, sizeof(static_vector_t));

    for (int i = 0; i < table->len; i++)
        memcpy(&tmp.vec[i], &table->vec[key_arr[i].ind], sizeof(vector_type));
    
    memcpy(table, &tmp, sizeof(static_vector_t));
}

int init_sort(static_vector_t *table, int type, int is_key)
{
    if (!table->len)
    {
        puts("Таблица пуста.");
        return NO_ERROR;
    }
    struct timeval start_time_s, end_time_s;
    key_t key_arr[STATIC_VECTOR_SIZE];

    #if DEBUG
    printf("is key: %d\n", is_key);
    #endif

    if (is_key)
        key_cpy(table, key_arr);
    
    START_MEASURING();

    switch (type)
    {
    case QUICK_SORT:
        if (is_key)
        {
            qsort(key_arr, table->len, sizeof(key_t), cost_cmp_key);
        }
        else
        {
            qsort(table->vec, table->len, sizeof(vector_type), cost_cmp);
        }
        
        break;
    
    case BUBBLE_SORT:
        if (is_key)
        {
            bubsort(key_arr, table->len, sizeof(key_t), cost_cmp_key);
        }
        else
        {
            bubsort(table->vec, table->len, sizeof(vector_type), cost_cmp);
        }

        break;
    }

    END_MEASURING();

    if (is_key)
    {
        key_to_table_cpy(table, key_arr);
        #if DEBUG
        for (int i = 0; i < table->len; i++)
            printf("#%d ind = %d cost = %d\n", i, key_arr[i].ind, key_arr[i].key);
        #endif
    }
    
    SHOW_TIME();
    
    return NO_ERROR;
}