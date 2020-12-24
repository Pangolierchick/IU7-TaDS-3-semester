#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "defines.h"
#include "io.h"
#include "matrix.h"

int main(void)
{
    print_hello();

    int cmd;
    
    matrix_t *matrix = NULL;
    vec_t    *vector = NULL;
    
    vec_t *def_mult_res = NULL;
    sparse_matrix_t *s_mult_res = NULL;

    int last_mult = 0;

    while (1)
    {
        cmd = -1;
        print_help();
        printf("\nВведите комманду: ");
        fpurge(stdin);
        scanf("%d", &cmd);

        int res;

        switch (cmd)
        {
        case INPUT_MATRIX:
            res = input_matrix(&matrix);
            break;
        case GEN_RND_MATRIX:
            res = rnd_matrix(&matrix);
            break;

        case INPUT_VECTOR:
            res = input_vector(&vector);
            break;

        case GEN_RND_VECTOR:
            res = rnd_vector(&vector);
            break;
        
        case DEFAULT_MULT:
            res = def_mult(vector, matrix, &def_mult_res);
            
            last_mult = res ? EMPTY_RES : DEFAULT_MULT;
            break;
        
        case SPARSE_MULT:
            res = sparse_mult(vector, matrix, &s_mult_res);
            
            last_mult = res ? EMPTY_RES : SPARSE_MULT;
            break;
        
        case PRINT_RES:
            if (last_mult == DEFAULT_MULT)
            {
                res = print_vector(def_mult_res);
            }
            else if (last_mult == SPARSE_MULT)
            {
                res = print_sparse_res(s_mult_res);
            }
            else
                res = EMPTY_RES;

            break;

        case CLEAN_SCREEN:
            system("clear");
            break;

        case PRINT_MATRIX:
            res = print_matrix(matrix);
            break;

        case PRINT_VECTOR:
            res = print_vector(vector);
            break;

        case EXIT_CMD:
            puts("Прощайте.");
            goto clean;

        case CURRENT_MEM:
            DBG_PRINT("%s\t%lld Bytes\n", YELLOW_MSG("Current allocated memory: "), allocated_mem);
            res = NO_ERROR;
            break;

        default:
            PRINT_ERROR("%s", "Была введена неверная команда.");
            res = 0;
            break;
        }

        if (res == ALLOCATION_FAILED)
        {
            PRINT_ERROR("alloc failed: %d", res);
            return ALLOCATION_FAILED;
        }

        if (res)
        {
            PRINT_ERROR("%s", mystrerr(res));
            res = 0;
        }
    }



clean:
    clean_matrix(&matrix);
    clean_vector(&vector);
    clean_vector(&def_mult_res);
    clean_sparse(&s_mult_res);

    return EXIT_SUCCESS;
}
