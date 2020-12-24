#ifndef __STATIC_VECTOR_H__
#define __STATIC_VECTOR_H__

#include "defines.h"

#define STATIC_VECTOR_SIZE      2000 // replace it with needed value 

typedef struct static_vector_s
{
    vector_type vec[STATIC_VECTOR_SIZE];
    int len;
} static_vector_t;

typedef int vec_error_t;

#define NO_ERROR                    0
#define INDEX_OUT_OF_BOUNDS         30
#define VECTOR_UNDERFLOW            31
#define VECTOR_OVERFLOW             32

extern vec_error_t vec_errno;

vec_error_t pop(static_vector_t *vec, const int ind, vector_type *dst);
vec_error_t insert(static_vector_t *vec, const vector_type val, const int ind);
vec_error_t append(static_vector_t *vec, const vector_type val);
vec_error_t get(const static_vector_t *vec, const int ind, vector_type *dst);
vec_error_t set(static_vector_t *vec, const int ind, const vector_type src);
void init_vector(static_vector_t *vec);

#endif // __STATIC_VECTOR_H__