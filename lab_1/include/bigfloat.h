#ifndef __BIGFLOAT_H__
#define __BIGFLOAT_H__

#include <stdlib.h>
#include <stdint.h>
#include "error.h"

#define SUB_ERROR                       30
#define MANTISSA_LENGTH                 31
#define EXP_LENGTH                      5

#define TRUE                            1
#define FALSE                           0

#define SIGN(num) (num / abs(num))

typedef enum sign_e
{
    NEGATIVE = 0,
    POSITIVE = 1
} sign_t;

typedef struct bigfloat_s
{
    sign_t mant_sign;
    char mantissa[MANTISSA_LENGTH];
    char dot_pos;
    int exponent;
    int mant_len;
} bigfloat_t;

void str_to_int(const char *str, int *dst, int len);
void remove_last_zeroes(bigfloat_t *f);
void dump_print(const bigfloat_t src);
void normalise(bigfloat_t *src);
my_error_t division(bigfloat_t *divided, bigfloat_t *diviser, bigfloat_t *res);
int count_div_iters(bigfloat_t *divided, bigfloat_t *diviser);
my_error_t mant_sub(bigfloat_t *divided, bigfloat_t *diviser);
int is_nil(const bigfloat_t num);
void right_shift(bigfloat_t *f);
void remove_lead_zeros(bigfloat_t *src);



#endif //__BIGFLOAT_H__