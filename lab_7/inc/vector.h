#ifndef __STATIC_VECTOR_H__
#define __STATIC_VECTOR_H__

#define INIT_SIZE                  10

#define GENERIC_POINTER(p)              (void *) p

typedef int vector_type;

typedef struct vector_s
{
    vector_type *vec;
    int len;
    int alloc_len;
} vector_t;

typedef int vec_error_t;

#define NO_ERROR                    0
#define INDEX_OUT_OF_BOUNDS         10
#define VECTOR_UNDERFLOW            11
#define VECTOR_OVERFLOW             12
#define ALLOCATION_FAULT            13

extern vec_error_t vec_errno;

vec_error_t vec_pop(vector_t *vec, const int ind, vector_type *dst);
vec_error_t vec_insert(vector_t *vec, const vector_type *val, const int ind);
vec_error_t vec_append(vector_t *vec, const vector_type *val);
vec_error_t vec_get(const vector_t *vec, const int ind, vector_type *dst);
vec_error_t vec_set(vector_t *vec, const int ind, const vector_type *src);
vector_t *init_vector(void);
void clean_vector(vector_t *vec);

int vec_find(vector_t *vec, vector_type *val);

vector_type *init_vector_type(void);
void clean_vector_type(vector_type *elem);

#endif // __STATIC_VECTOR_H__
