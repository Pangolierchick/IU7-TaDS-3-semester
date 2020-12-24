#include <stdlib.h>
#include "errors.h"
#include "fileio.h"

#define INIT_LEN            100


int lins(int *arr, int len, int key)
{
    for (int i = 0; i < len; i++)
        if (arr[i] == key)
            return i;
    return -1;
}

int read_from_file(FILE *f, int **arr, int *len)
{
    if (get_file_size(f) == 0)
        return EMPTY_FILE;

    *len = INIT_LEN;
    
    *arr = malloc(sizeof(int) * *len);

    if (arr == NULL)
        return ALLOC_ERROR;
    
    int rr = 1;
    int i = 0;

    while (rr == 1)
    {
        rr = fscanf(f, "%d", *arr + i);

        i += (rr == 1);

        if (i >= *len)
        {
            *len *= 2;
            *arr = realloc(*arr, sizeof(int) * *len);
            
            if (*arr == NULL)
                return ALLOC_ERROR;
        }
    }

    *len = i;

    if (rr != EOF)
    {
        free(*arr);
        return BAD_FILE;
    }
    
    return EXIT_SUCCESS;
}

int get_file_size(FILE *f)
{
    fseek(f, 0L, SEEK_END);
    long size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    return size;
}

int find_from_file(FILE *f, int key)
{
    rewind(f);
    int curr_val;
    int res = 1;
    int i = 1;

    res = fscanf(f, "%d", &curr_val);

    while (res == 1 && curr_val != key && i++ > 0)
        res = fscanf(f, "%d", &curr_val);
    
    rewind(f);

    return i;
}
