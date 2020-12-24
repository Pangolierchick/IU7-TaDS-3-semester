#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "matrix.h"
#include "error.h"
#include "io.h"
#include "vector.h"

uint64_t allocated_mem;

struct timespec start_time_s, end_time_s;

matrix_t *init_matrix(size_t x, size_t y)
{
    int **matrix = (int **)malloc(y * sizeof(int *));

    if (matrix == NULL)
        return NULL;

    for (int i = 0; i < y; i++)
    {
        matrix[i] = (int *)malloc(x * sizeof(int));

        if (matrix[i] == NULL)
            return NULL;
    }

    matrix_t *dst = malloc(sizeof(matrix_t));

    if (dst == NULL)
        return NULL;

    dst->matrix = matrix;
    dst->x = x;
    dst->y = y;

    long int last_alloc = allocated_mem;

    allocated_mem += y * sizeof(int *);
    allocated_mem += x * sizeof(int) * y;
    allocated_mem += sizeof(matrix_t);

    INFO_PRINT("Current alloc mem: %lld Bytes\n", allocated_mem - last_alloc);

    return dst;
}

void clean_matrix(matrix_t **matrix)
{
    DBG_PRINT("Matrix p: %p\n", *matrix);

    if (*matrix != NULL)
    {
        allocated_mem -= (*matrix)->y * sizeof(int *);
        allocated_mem -= (*matrix)->y * (*matrix)->x * sizeof(int);
        allocated_mem -= sizeof(matrix_t);

        for (int i = 0; i < (*matrix)->y; i++)
            free((*matrix)->matrix[i]);

        free((*matrix)->matrix);

        free(*matrix);
        *matrix = NULL;

        DBG_PRINT("Current alloc mem: %lld Bytes\n", allocated_mem);
        DBG_PUTS("Cleaned");
    }
}

int input_matrix(matrix_t **matrix)
{
    int x;
    int y;

    fpurge(stdin);

    printf("Введите кол-во строк: ");
    if (scanf("%d", &y) != 1)
        return BAD_ROW;

    printf("Введите кол-во столбцов: ");
    if (scanf("%d", &x) != 1)
        return BAD_CLM;

    if (x <= 0 || y <= 0)
        return BAD_ROW;

    if (x > RECOMMENDED_TOP_X_LIMIT || y > RECOMMENDED_TOP_Y_LIMIT)
        if (!ask_y_n("Введен большой размер матрицы, продолжить ввод?"))
            return NO_ERROR;

    clean_matrix(matrix);
    *matrix = init_matrix(x, y);

    if (*matrix == NULL)
        return ALLOCATION_FAILED;

    puts("Начинаем ввод матрицы.");
    fpurge(stdin);

    DBG_PRINT("row = %d\tclm = %d\n", (*matrix)->y, (*matrix)->x);

    for (int row = 0; row < (*matrix)->y; row++)
    {
        for (int clm = 0; clm < (*matrix)->x; clm++)
        {
            printf("matrix[%d][%d] = ", row, clm);
            if (scanf("%d", &(*matrix)->matrix[row][clm]) != 1)
                return 1;
        }
    }

    return NO_ERROR;
}

int print_matrix(matrix_t *matrix) // fragile
{
    if (matrix == NULL)
        return EMPTY_MATRIX;

    int print_all = 1;
    if (matrix->x > RECOMMENDED_TOP_X_LIMIT || matrix->y > RECOMMENDED_TOP_Y_LIMIT)
    {
        print_all = ask_y_n("Размер матрицы очень большой. Напечатать ее полностью?:");
    }

    puts("-------- Матрица --------");

    if (print_all)
    {
        for (int row = 0; row < matrix->y; row++)
        {
            for (int clm = 0; clm < matrix->x; clm++)
                printf("%d\t", matrix->matrix[row][clm]);
            printf("\n");
        }
    }
    else
    {
        if (matrix->y > RECOMMENDED_TOP_Y_LIMIT && matrix->x < RECOMMENDED_TOP_X_LIMIT)
        {
            for (int row = 0; row < NUMBER_OF_SHORT_PRINT; row++)
            {
                for (int clm = 0; clm < matrix->x; clm++)
                    printf("%d\t", matrix->matrix[row][clm]);
                printf("\n");
            }

            printf("\n");
            for (int row = 0; row < NUMBER_OF_SHORT_PRINT * 2 + 1; row++)
                printf(".\t");
            printf("\n");
            for (int row = 0; row < NUMBER_OF_SHORT_PRINT * 2 + 1; row++)
                printf(".\t");
            printf("\n\n");

            for (int row = matrix->y - NUMBER_OF_SHORT_PRINT; row < matrix->y; row++)
            {
                for (int clm = 0; clm < matrix->x; clm++)
                    printf("%d\t", matrix->matrix[row][clm]);
                printf("\n");
            }
        }
        else if (matrix->y < RECOMMENDED_TOP_Y_LIMIT && matrix->x > RECOMMENDED_TOP_X_LIMIT)
        {
            for (int row = 0; row < matrix->y; row++)
            {
                for (int clm = 0; clm < NUMBER_OF_SHORT_PRINT; clm++)
                    printf("%d\t", matrix->matrix[row][clm]);

                printf(".  .\t");

                for (int clm = matrix->x - NUMBER_OF_SHORT_PRINT; clm < matrix->x; clm++)
                    printf("%d\t", matrix->matrix[row][clm]);

                printf("\n");
            }
        }
        else
        {
            for (int row = 0; row < NUMBER_OF_SHORT_PRINT; row++)
            {
                for (int clm = 0; clm < NUMBER_OF_SHORT_PRINT; clm++)
                    printf("%d\t", matrix->matrix[row][clm]);

                printf(". .\t");

                for (int clm = matrix->x - NUMBER_OF_SHORT_PRINT; clm < matrix->x; clm++)
                    printf("%d\t", matrix->matrix[row][clm]);

                printf("\n");
            }

            printf("\n");
            for (int row = 0; row < NUMBER_OF_SHORT_PRINT * 2 + 1; row++)
                printf(".\t");
            printf("\n");
            for (int row = 0; row < NUMBER_OF_SHORT_PRINT * 2 + 1; row++)
                printf(".\t");
            printf("\n\n");

            for (int row = matrix->y - NUMBER_OF_SHORT_PRINT; row < matrix->y; row++)
            {
                for (int clm = 0; clm < NUMBER_OF_SHORT_PRINT; clm++)
                    printf("%d\t", matrix->matrix[row][clm]);

                printf(". .\t");

                for (int clm = matrix->x - NUMBER_OF_SHORT_PRINT; clm < matrix->x; clm++)
                    printf("%d\t", matrix->matrix[row][clm]);

                printf("\n");
            }
        }
    }

    return NO_ERROR;
}

int rnd_matrix(matrix_t **matrix)
{
    int percent;
    int x;
    int y;

    fpurge(stdin);

    printf("Введите кол-во строк: ");
    if (scanf("%d", &y) != 1)
        return BAD_ROW;

    printf("Введите кол-во столбцов: ");
    if (scanf("%d", &x) != 1)
        return BAD_CLM;

    if (x <= 0 || y <= 0)
        return BAD_ROW;

    printf("Введите процент нулей: (0%% - 100%%): ");
    if (scanf("%d", &percent) != 1)
        return BAD_PERCENT;

    if (percent < 0 || percent > 100)
        return BAD_PERCENT;

    clean_matrix(matrix);

    if ((*matrix = init_matrix(x, y)) == NULL)
        return ALLOCATION_FAILED;

#if INFO
    const char symb[] = {'|', '/', '-', '\\'};
    int i = 0;
    size_t j = 0;
    const size_t total = (size_t)y * x;
    START_MEASURING();
#endif

    for (size_t row = 0; row < y; row++)
    {
        for (size_t clm = 0; clm < x; clm++)
        {
#if INFO
            j++;
#endif
            (*matrix)->matrix[row][clm] = GET_RND_ELEM(percent);
        }
        INFO_PRINT("Generating (est: %ld%%:%d%%) %c\r", (size_t)(((double)j / ((double)(total))) * 100), 100, symb[i % 4]);

#if INFO
        if (row % 400 == 0)
            i++;
#endif
    }

#if DEBUG
    END_MEASURING();
    SHOW_TIME();
#endif

    return NO_ERROR;
}

sparse_matrix_t *convert_matrix(const matrix_t *matrix)
{
#if DEBUG
    START_MEASURING();
#endif

    int non_zero = 0;

    DBG_PRINT("Counting non zeroes %25s\t\t", "..........");
    

    for (int row = 0; row < matrix->y; row++)
        for (int clm = 0; clm < matrix->x; clm++)
            if (matrix->matrix[row][clm])
                non_zero++;

#if DEBUG
    printf("%d\n", non_zero);
#endif

    sparse_matrix_t *sparse = init_sparse(non_zero, non_zero, matrix->y + 1);
    sparse->descr->arr[0] = 0;

    int elem = 0;
    int curr_line = 0;

    DBG_PRINT("Converting %34s\t\t", "..........");
    for (int row = 0; row < matrix->y; row++)
    {
        for (int clm = 0; clm < matrix->x; clm++)
        {
            if (matrix->matrix[row][clm])
            {
                sparse->clms->arr[elem] = clm;
                sparse->non_zero->arr[elem] = matrix->matrix[row][clm];

                elem++;
                curr_line++;
            }
        }
        sparse->descr->arr[row + 1] = curr_line;
    }
#if DEBUG
    puts("Done");
#endif

#if DEBUG
    END_MEASURING();
    puts(YELLOW_MSG("Время конвертации матрицы"));
    SHOW_TIME();
#endif
    return sparse;
}

sparse_matrix_t *init_sparse(size_t non_zero, size_t clms, size_t descr)
{
    sparse_matrix_t *sparse = NULL;

    int last_alloc = allocated_mem;

    sparse = malloc(sizeof(sparse_matrix_t));

    if (!sparse)
        return NULL;


    sparse->non_zero = init_vector(non_zero);
    sparse->clms = init_vector(clms);
    sparse->descr = init_vector(descr);

    allocated_mem += sizeof(sparse_matrix_t);

    if (!sparse->non_zero || !sparse->clms || !sparse->descr)
        return NULL;

    INFO_PRINT("Init sparse. Alloc mem: %lld Bytes\n", allocated_mem - last_alloc);

    return sparse;
}

void clean_sparse(sparse_matrix_t **sparse)
{
    DBG_PRINT("Cleaning sparse: %p\n", *sparse);

    if (*sparse)
    {
        allocated_mem -= sizeof(sparse_matrix_t);

        clean_vector(&(*sparse)->clms);
        clean_vector(&(*sparse)->descr);
        clean_vector(&(*sparse)->non_zero);

        free((*sparse));
        *sparse = NULL;
        DBG_PRINT("Current alloc mem: %lld Bytes\n", allocated_mem);
    }

}

/*
             (1, 2, 3)
(1, 2, 3) *  (4, 5, 6) = ()
             (7, 8, 9)

*/
int def_mult(const vec_t *vec, const matrix_t *matrix, vec_t **res)
{
    if (vec == NULL || matrix == NULL)
        return NOT_ENOUGH_OPERANDS;

    #if INFO
    const char symb[] = {'|', '/', '-', '\\'};
    int sym = 0;
    #endif

    if (vec->len != matrix->y)
        return ROWS_NOT_EQUAL_COLUMNS;
    
    START_MEASURING();

    clean_vector(res);
    *res = init_vector(matrix->x);

    for (int i = 0; i < matrix->x; i++)
    {
        int elem = 0;
        for (int k = 0; k < matrix->y; k++)   
        {
            elem += vec->arr[k] * matrix->matrix[k][i];
            #if INFO
            if (k % 75 == 0)
            {
                printf("\rMultuplying %c", symb[sym % 4]);
                sym++;
            }
            #endif
        }


        (*res)->arr[i] = elem;
    }
    END_MEASURING();
    SHOW_TIME();

    printf(YELLOW_MSG("Памяти использовано: %ld B"), matrix->y * sizeof(int *) + matrix->x * sizeof(int) * matrix->y + sizeof(matrix_t));

    return NO_ERROR;
}



matrix_t *transpose(matrix_t *matrix)
{
    matrix_t *new_matrix = init_matrix(matrix->y, matrix->x);

    for (int i = 0; i < matrix->y; i++)
        for (int j = 0; j < matrix->x; j++)
            new_matrix->matrix[j][i] = matrix->matrix[i][j];
    
    return new_matrix;
}

/*
    0 0 0
    1 1 1
    1 1 1

    1 1 1 1 1 1
    1 2 3 1 2 3
    0 0 3 6



    1 0 1
    0 0 0
    1 1 1

    1 1 1 1 1 1
    1 2 3 1 2 3
    0 3 3 6

    1 2 3       1 4 2
    4 0 6   --> 2 0 1
    2 1 0       3 6 0

    1 0 3

    -----------------
    1 4 2 2 1 3 6
    0 1 2 0 2 0 1
    0 3 5 7
    -----------------

    -----------------
    1 3
    0 2
    -----------------
*/

int sparse_mult(const vec_t *vec, matrix_t *matrix, sparse_matrix_t **res)
{
    if (vec == NULL || matrix == NULL)
        return NOT_ENOUGH_OPERANDS;
    
    if (vec->len != matrix->y)
        return ROWS_NOT_EQUAL_COLUMNS;


    START_MEASURING();
    matrix_t *new_matrix = transpose(matrix);
    END_MEASURING();

    long secs = end_time_s.tv_sec - start_time_s.tv_sec;
    long nsecs = end_time_s.tv_nsec - start_time_s.tv_nsec;

    sparse_vec_t *svec = convert_vec(vec);
    
    long alloced = allocated_mem;
    sparse_matrix_t *smatrix = convert_matrix(new_matrix);
    long last_alloced = allocated_mem;

    if (svec->val->len == 0 || smatrix->non_zero->len == 0)
        return EMPTY_MULT;

    if (svec == NULL || smatrix == NULL)
        return ALLOCATION_FAILED;

    START_MEASURING();

    clean_sparse(res);
    *res = init_sparse(smatrix->non_zero->len, smatrix->clms->len, matrix->x + 1);

    if (*res == NULL)
        return ALLOCATION_FAILED;
    

    (*res)->descr->arr[0] = 0;
    (*res)->non_zero->len = 0;
    (*res)->clms->len = 0;
    (*res)->descr->len = 2;

    int curr_sum;
    int i;

    int start_ind;
    int len;

    for (i = 0; i < matrix->x; i++)
    {
        curr_sum = 0;
        start_ind = smatrix->descr->arr[i];
        len = smatrix->descr->arr[i + 1] - smatrix->descr->arr[i];

        while (start_ind < len + smatrix->descr->arr[i])
        {
            curr_sum += smatrix->non_zero->arr[start_ind] * vec->arr[smatrix->clms->arr[start_ind]];
            start_ind++;
        }
        
        if (curr_sum)
        {
            (*res)->non_zero->arr[(*res)->non_zero->len++] = curr_sum;
            (*res)->clms->arr[(*res)->clms->len++] = i;
        }
    }

    (*res)->descr->arr[1] = (*res)->non_zero->len;

    END_MEASURING();
    printf("Время умножения %ld s %ld ns\n", end_time_s.tv_sec - start_time_s.tv_sec + secs, end_time_s.tv_nsec - start_time_s.tv_nsec + nsecs);
    printf(YELLOW_MSG("Памяти использовано: %ld B"), last_alloced - alloced);


    clean_svec(&svec);
    clean_sparse(&smatrix);
    clean_matrix(&new_matrix);

    return NO_ERROR;
}

int print_sparse_res(const sparse_matrix_t *res)
{
    printf("Non zero elems: ");
    for (int i = 0; i < res->non_zero->len; i++)
        printf("%5d ", res->non_zero->arr[i]);
    printf("\n");

    printf("Columns: ");
    for (int i = 0; i < res->clms->len; i++)
        printf("%2d ", res->clms->arr[i]);
    printf("\n");

    printf("IA: ");
    for (int i = 0; i < res->descr->len; i++)
        printf("%2d ", res->descr->arr[i]);
    printf("\n");
    return NO_ERROR;
}
