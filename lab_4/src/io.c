#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "io.h"
#include "array_stack.h"
#include "list_stack.h"
#include "stack.h"
#include "error.h"
#include "logger.h"
#include "timer.h"

struct timespec start_time_s, end_time_s;

#define PRINT_ARR(arr, len)                     \
            for (int i = 0; i < len; i++)       \
                printf("%d ", arr[i]);          \
            printf("\n");                       \

void print_menu(void)
{
    printf("\t%d -- Ввести последовательность\n", INPUT_SEQUENCE);
    printf("\t%d -- Напечатать состояние стека-массива\n", PRINT_STACK_ARRAY);
    printf("\t%d -- Добавить элемент в стек-массив\n", ADD_ELEM_ARRAY);
    printf("\t%d -- Напечатать состояние стека-списка\n", PRINT_STACK_LIST);
    printf("\t%d -- Добавить элемент в стек-список\n", ADD_ELEM_LIST);
    printf("\t%d -- Напечатать в обратном порядке убывающие подпоследовательности\n", FIND_SEQUENCES);
    printf("\t%d -- Напечатать меню\n", PRINT_HELP);
    printf("\t%d -- Очистить экран\n", CLEAR_SCREEN);
    printf("\t%d -- Считать последовательность из файла\n", READ_FROM_FILE);
    printf("\t%d -- Выход из программы\n", EXIT);
}

int input_sequence(arr_stack_t **astack, list_stack_t **lstack)
{
    int len;

    printf("Введите длину последовательности. Максимальная длина %d: ", LIST_MAX_LEN);
    if (scanf("%d", &len) != 1)
        return BAD_SEQ_LEN;

    if (len <= 0)
        return BAD_SEQ_LEN;

    if (len > LIST_MAX_LEN)
        return TOO_BIG_LEN;

    CLEAR(astack);
    CLEAR(lstack);

    *astack = arr_init_stack();
    *lstack = list_init_stack();

    if (*astack == NULL || lstack == NULL)
        return ALLOC_FAILED;

    int i = 0;
    int elem;

    while (i < len)
    {
        printf("Введите %d:%d элемент: ", i + 1, len);
        if (scanf("%d", &elem) != 1)
            return BAD_ELEM;

        APPEND(*astack, elem);

        i++;
    }

    for (int j = 0; j < len; j++)
        APPEND(*lstack, (*astack)->arr[j]);

    return NO_ERROR;
}

/*
    3      3
    2      2
    4      4
    5      5
    1      1

    2 3

    1 5

    3 2
    2 4
    4 5
    5 1
    1 -1
*/

int find_in_lstack(list_stack_t *lstack)
{
    puts(YELLOW_MSG("Поиск подпоследовательностей в стеке-списке"));
    const int stack_len = get_len(lstack);

    if (stack_len == 0)
        return EMPTY_STACK;

    int *arr = malloc(sizeof(int) * stack_len);

    if (arr == NULL)
        return ALLOC_FAILED;
    
    list_stack_t *cpy_stack = CPY(lstack);
    int len = 0;

    if (cpy_stack == NULL)
        return -1;
    
    int curr_val;

    list_err = 0;
    arr[len++] = POP(cpy_stack);
    bool printed = false;
    int seq_num = 1;

    while (!list_err)
    {
        curr_val = POP(cpy_stack);

        DBG_PRINT("len %d\tcurr val %d\n", len, curr_val);

        if (list_err)
            break;

        if (arr[len - 1] < curr_val)
            arr[len++] = curr_val;
        else
        {
            if (len > 1)
            {   
                printed = true;
                printf("Последовательность #%d\t", seq_num++);
                PRINT_ARR(arr, len);
            }
            len = 1;
            arr[0] = curr_val;
        }
    }

    if (len > 1)
    {   
        printed = true;
        printf("Последовательность #%d\t", seq_num++);
        PRINT_ARR(arr, len);
    }

    if (!printed)
        puts("Не найдено такой подпоследовательности.");

    CLEAR(&cpy_stack);
    free(arr);

    return NO_ERROR;
}

int find_in_astack(arr_stack_t *astack)
{
    puts(YELLOW_MSG("Поиск подпоследовательностей в стеке-массиве"));

    if (astack->len == 0)
        return EMPTY_STACK;

    int *arr = malloc(sizeof(int) * astack->len);

    if (arr == NULL)
        return ALLOC_FAILED;

    arr_stack_t *cpy_stack = CPY(astack);
    int len = 0;

    if (cpy_stack == NULL)
        return -1;

    list_err = 0;
    int curr_val;

    arr_err = 0;
    arr[len++] = POP(cpy_stack);
    bool printed = false;
    int seq_num = 1;

    while (!arr_err)
    {
        curr_val = POP(cpy_stack);

        DBG_PRINT("len %d\tcurr val %d\tlast val %d\n", len, curr_val, arr[len - 1]);

        if (arr_err)
            break;

        if (arr[len - 1] < curr_val)
            arr[len++] = curr_val;
        else
        {
            if (len > 1)
            {   
                printed = true;
                printf("Последовательность #%d\t", seq_num++);
                PRINT_ARR(arr, len);
            }
            len = 1;
            arr[0] = curr_val;
        }
    }

    if (len > 1)
    {   
        printed = true;
        printf("Последовательность #%d\t", seq_num++);
        PRINT_ARR(arr, len);
    }

    if (!printed)
        puts("Не найдено такой подпоследовательности.");

    CLEAR(&cpy_stack);
    free(arr);

    return NO_ERROR;
}

int find_seq(arr_stack_t *astack, list_stack_t *lstack)
{
    if (astack == NULL || lstack == NULL)
        return NOT_INPUTTED;

    START_MEASURING();

    int res = find_in_astack(astack);


    END_MEASURING();

    if (res)
        return res;

    SHOW_TIME("Время поиска в стеке-массиве: ");

    int array_sec = end_time_s.tv_sec - start_time_s.tv_sec;
    long array_nsec = end_time_s.tv_nsec - start_time_s.tv_nsec;

    START_MEASURING();

    res = find_in_lstack(lstack);

    END_MEASURING();

    if (res)
        return res;

    SHOW_TIME("Время поиска в стеке-списке: ");

    int list_sec = end_time_s.tv_sec - start_time_s.tv_sec;
    long list_nsec = end_time_s.tv_nsec - start_time_s.tv_nsec;

    if (array_sec * (int) 10e6 + array_nsec < list_sec * (int) 10e6 + list_nsec)
    {
        printf("Стек-массив оказался быстрее на %dс %dнс\n", abs(list_sec - array_sec), abs(list_nsec - array_nsec));
    }
    else
    {
        printf("Стек-список оказался быстрее на %dс %dнс\n", abs(list_sec - array_sec), abs(list_nsec - array_nsec));
    }

    printf(YELLOW_MSG("Потребление памяти:\n"));
    printf("\tСписок:\t%6ld Б\n", lstack->len * sizeof(node_t));
    printf("\tМассив:\t%6ld Б\n", astack->len * sizeof(int));

    return NO_ERROR;
}

int read_seq_from_file(arr_stack_t **astack, list_stack_t **lstack)
{
    const int fname_len = 30;
    char fsrc[fname_len];
    
    printf("Введите имя файла: ");
    scanf("%30s", fsrc);
    
    printf("Открытие файла\t\t........\t\t");
    FILE *f = fopen(fsrc, "r");

    if (f == NULL)
    {
        puts("Ошибка.");
        return UNEXISTING_FILE;
    }
    puts("Готово.");
    
    CLEAR(astack);
    CLEAR(lstack);

    *astack = arr_init_stack();
    *lstack = list_init_stack();

    if (astack == NULL || lstack == NULL)
        return ALLOC_FAILED;
    
    int res = 1;

    printf("Считывание\t\t........\t\t");

    int len = 0;

    while (res != EOF)
    {
        int curr_val;
        res = fscanf(f, "%d", &curr_val);
        
        if (res == 0)
        {
            CLEAR(astack);
            CLEAR(lstack);
            puts("Ошибка.");
            return BAD_FILE;
        }

        if (res == 1)
        {
            int app_res = 0;
            app_res = APPEND(*astack, curr_val);
            app_res |=  APPEND(*lstack, curr_val);

            if (app_res)
            {
                puts("Ошибка.");
                return STACK_OVERFLOW;
            }

            len++;
        }
    }

    puts("Готово.");

    fclose(f);

    return len == 0 ? EMPTY_FILE : NO_ERROR;
}

int add_elem_list(list_stack_t **list)
{
    printf("Введите элемент\n");

    if (*list == NULL)
        *list = list_init_stack();

    int elem;
    if (scanf("%d", &elem) != 1)
        return BAD_ELEM;

    return APPEND(*list, elem);
}

int add_elem_arr(arr_stack_t **arr)
{
    printf("Введите элемент\n");

    if (*arr == NULL)
        *arr = arr_init_stack();

    int elem;
    if (scanf("%d", &elem) != 1)
        return BAD_ELEM;

    return APPEND(*arr, elem);
}
