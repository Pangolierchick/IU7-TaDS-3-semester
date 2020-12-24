#include "funcs.h"

int get_file_size(FILE *f)
{
    fseek(f, 0L, SEEK_END);
    long size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    return size;
}
