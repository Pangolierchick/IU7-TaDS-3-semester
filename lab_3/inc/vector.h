#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <string.h>
#include "defines.h"

typedef int vec_type_t;

typedef struct vec_s
{
    vec_type_t *arr;
    size_t len;
    size_t alloc_len;
} vec_t;

typedef struct sparse_vec_s
{
    vec_t *val;
    vec_t *ind;
} sparse_vec_t;

vec_t *init_vector(size_t len);
void clean_vector(vec_t **vec);

int input_vector(vec_t **vec);
int rnd_vector(vec_t **vec);

sparse_vec_t *convert_vec(const vec_t *vec);

int print_vector(vec_t *vec);
void clean_svec(sparse_vec_t **svec);

#endif //__VECTOR_H__