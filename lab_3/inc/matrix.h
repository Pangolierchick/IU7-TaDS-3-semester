#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "vector.h"
#include "defines.h"

extern uint64_t allocated_mem;

extern struct timespec start_time_s, end_time_s; // for timer purposes

// CLOCK_REALTIME
// CLOCK_PROCESS_CPUTIME_ID

#define START_MEASURING() clock_gettime(CLOCK_REALTIME, &start_time_s);
#define END_MEASURING() clock_gettime(CLOCK_REALTIME, &end_time_s);

#define SHOW_TIME() printf ("\n\n\x1b[33mВремя:\033[0m %lds %ldns\n", end_time_s.tv_sec - start_time_s.tv_sec, labs(end_time_s.tv_nsec - start_time_s.tv_nsec))

#define RECOMMENDED_TOP_X_LIMIT           15
#define RECOMMENDED_TOP_Y_LIMIT           15

#define TOP_ELEM_LIMIT                    100

#define NUMBER_OF_SHORT_PRINT             3

#define GET_VEC(vec, x, y, M)             vec[x + M * y]

typedef struct matrix_s
{
    int x;
    int y;

    int **matrix;
} matrix_t;

typedef struct sparse_matrix_s
{
    vec_t *non_zero;
    vec_t *clms;
    vec_t *descr;
} sparse_matrix_t;

matrix_t *init_matrix(size_t x, size_t y);
void clean_matrix(matrix_t **matrix);

int input_matrix(matrix_t **matrix);
int print_matrix(matrix_t *matrix);
int rnd_matrix(matrix_t **matrix);

sparse_matrix_t *init_sparse(size_t non_zero, size_t clms, size_t rows);
void clean_sparse(sparse_matrix_t **sparse);

sparse_matrix_t *convert_matrix(const matrix_t *matrix);

int def_mult(const vec_t *vec, const matrix_t *matrix, vec_t **res);
int sparse_mult(const vec_t *vec, matrix_t *matrix, sparse_matrix_t **res);

int print_sparse_res(const sparse_matrix_t *res);

#endif // __MATRIX_H__
