#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include "matrix.h"
#include "error.h"
#include "io.h"

vec_t *init_vector(size_t len)
{
    vec_t *vec = NULL;
    
    if (!(vec = malloc(sizeof(vec_t))))
        return NULL;
    
    if (!(vec->arr = malloc(sizeof(vec_type_t) * len)))
        return NULL;

    vec->alloc_len = len;
    vec->len = len;

    long int last_alloced = allocated_mem;

    // ----------------------------
    allocated_mem += (len * sizeof(vec_type_t));
    allocated_mem += sizeof(vec_t);
    // ----------------------------

    INFO_PRINT("Current alloc mem: %lld Bytes\n", allocated_mem - last_alloced);
    return vec;
}

void clean_vector(vec_t **vec)
{
    DBG_PRINT("Vector p: %p\n", *vec);
    if (*vec)
    {
        // ----------------------------
        allocated_mem -= ((*vec)->alloc_len * sizeof(vec_type_t));
        allocated_mem -= sizeof(vec_t);
        // ----------------------------

        free((*vec)->arr);
        free(*vec);
        *vec = NULL;

        DBG_PRINT("Current alloc mem: %lld Bytes\n", allocated_mem);
        DBG_PUTS("Cleaned");
    }
}

int input_vector(vec_t **vec)
{
    int len;

    fpurge(stdin);

    printf("Введите длину вектора: ");
    if (scanf("%d", &len) != 1)
        return BAD_VEC_LEN;
    
    if (len <= 0)
        return BAD_VEC_LEN;
    
    if (len > RECOMMENDED_TOP_X_LIMIT)
        if (!ask_y_n("Введен большой размер вектора, продолжить?"))
            return NO_ERROR;
    
    *vec = init_vector(len);

    if (*vec == NULL)
        return ALLOCATION_FAILED;
    
    for (int i = 0; i < (*vec)->len; i++)
    {
        printf("Элемент [%d] = ", i);
        if (scanf("%d", &(*vec)->arr[i]) != 1)
            return BAD_VEC_ELEM;
    }

    return NO_ERROR;
}

int rnd_vector(vec_t **vec)
{
    int len;

    fpurge(stdin);

    printf("Введите длину вектора: ");
    if (scanf("%d", &len) != 1)
        return BAD_VEC_LEN;
    
    if (len <= 0)
        return BAD_VEC_LEN;
    
    
    int percent;

    fpurge(stdin);

    printf("Введите процент нулей: (0%% - 100%%): ");
    if (scanf("%d", &percent) != 1)
        return BAD_PERCENT;

    if (percent < 0 || percent > 100)
        return BAD_PERCENT;
    
    clean_vector(vec);
    *vec = init_vector(len);

    if (*vec == NULL)
        return ALLOCATION_FAILED;

    for (int i = 0; i < len; i++)
        (*vec)->arr[i] = GET_RND_ELEM(percent);
    
    return NO_ERROR;
}

int print_vector(vec_t *vec)
{
    if (vec == NULL)
        return EMPTY_VECTOR;
    else
    {
        if (vec->len > RECOMMENDED_TOP_X_LIMIT)
        {
            if (!ask_y_n("Размер вектора очень большой. Напечатать полностью?"))
            {
                for (int i = 0; i < 5; i++)
                    printf("%d\t", vec->arr[i]);
                printf("..\t");
                for (int i = vec->len - 5; i < vec->len; i++)
                    printf("%d\t", vec->arr[i]);
                printf("\n");

                return NO_ERROR;
            }
        }

        for (int i = 0; i < vec->len; i++)
            printf("%d\t", vec->arr[i]);
        printf("\n");
    }

    return NO_ERROR;
}

sparse_vec_t *convert_vec(const vec_t *vec)
{
    int non_zero = 0;

    DBG_PRINT("Counting non zeroes %25s\t\t", "..........");

    for (int i = 0; i < vec->len; i++)
        if (vec->arr[i])
            non_zero++;
    
#if DEBUG
    printf("%d\n", non_zero);
#endif
    
    sparse_vec_t *svec = malloc(sizeof(sparse_vec_t));

    if (svec == NULL)
        return NULL;

    long int alloced = allocated_mem;
    
    svec->val = init_vector(non_zero);
    svec->ind = init_vector(non_zero);

    if (svec->val == NULL || svec->ind == NULL)
        return NULL;
    
    int ind = 0;
    for (int i = 0; i < vec->len; i++)
    {
        if (vec->arr[i])
        {
            svec->val->arr[ind] = vec->arr[i];
            svec->ind->arr[ind] = i;
            ind++;
        }
    }

    INFO_PRINT("Sparse vec inited. Mem alloced %lld\n", sizeof(sparse_matrix_t) + allocated_mem - alloced);

    return svec;
}

void clean_svec(sparse_vec_t **svec)
{
    DBG_PRINT("Sparsed vector p: %p\n", *svec);
    if (*svec)
    {
        clean_vector(&(*svec)->ind);
        clean_vector(&(*svec)->val);
        free(*svec);
        
        *svec = NULL;
    }
}
