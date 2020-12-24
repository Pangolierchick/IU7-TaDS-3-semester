#ifndef __MYIO_H__
#define __MYIO_H__

#include <stdio.h>
#include <stdint.h>
#include "error.h"
#include "bigfloat.h"

#define BUFF_SIZE 60

#define PRINT_ERROR()

void hello_print(void);
my_error_t parse_long_int(const char *src, bigfloat_t *dst);
my_error_t parse_float(char *str, bigfloat_t *dst);
my_error_t input_bigfloat(bigfloat_t *src);
my_error_t input_long_int(bigfloat_t *src);
my_error_t find_dot_ind(char *str, bigfloat_t *dst);
void print_res(bigfloat_t res);
#endif // __MYIO_H__