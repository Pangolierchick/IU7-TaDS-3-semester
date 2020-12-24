#ifndef __FILEIO_H__
#define __FILEIO_H__

#include <stdio.h>

int read_from_file(FILE *f, int **arr, int *len);
int get_file_size(FILE *f);
int lins(int *arr, int len, int key);
int find_from_file(FILE *f, int key);

#endif // __FILEIO_H__
