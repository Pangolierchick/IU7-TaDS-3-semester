#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include "include/bigfloat.h"
#include "include/error.h"

#define MAX(a, b) (a > b ? a : b)

void remove_last_zeroes(bigfloat_t *f)
{
    int len = f->mant_len - 1;
    while (!f->mantissa[len])
    {
        len--;
        f->exponent++;
    }

    f->mant_len = len + 1;
}

void mant_offset(bigfloat_t *f, int offset)
{
    for (int i = 0; i < MANTISSA_LENGTH - 1 - offset; ++i)
        f->mantissa[i] = f->mantissa[i + offset];
    
    for (int i = MANTISSA_LENGTH - 1 - offset; i < MANTISSA_LENGTH - 1; ++i)
        f->mantissa[i] = 0;
}

void right_shift(bigfloat_t *f)
{
    if (f->mantissa[0] != 0)
    {
        for (int i = MANTISSA_LENGTH - 1; i > 0; i--)
            f->mantissa[i] = f->mantissa[i - 1];
        f->mantissa[0] = 0;
        f->mant_len++;
        f->dot_pos--;
    }
}

int is_greater(const bigfloat_t *f1, const bigfloat_t *f2)
{
    for (int i = 0; i < MANTISSA_LENGTH - 1; i++)
    {
        if ((f1->mantissa[i] - f2->mantissa[i]) > 0)
            return TRUE;
        if ((f1->mantissa[i] - f2->mantissa[i]) < 0)
        {
            return FALSE;
        }
    }

    return TRUE;
}

void str_to_int(const char *str, int *dst, int len)
{
    int i = strlen(str) - 1;
    
    int sign = SIGN(*dst);
    *dst = 0;
    
    for (int j = 0; j < len; j++)
        *dst += ((int) pow(10, j)) * (str[i--] - '0');
    
    *dst *= sign;
}

static void array_shift_left(char *arr, int shift)
{
    if (shift > 0)
        for (int i = 1; i < MANTISSA_LENGTH - 1; ++i)
            arr[i - shift] = arr[i];
}

void mantissa_shift_left(bigfloat_t *num, int shift)
{
    if (shift <= 0)
        return;
    for (int i = 1; i < MANTISSA_LENGTH - 1; i++)
        num->mantissa[i - shift] = num->mantissa[i];
}

void remove_lead_zeros(bigfloat_t *src)
{
    int i = src->mant_len - 1;
    while (!src->mantissa[i--])
    {
        src->mant_len--;
        if (i < src->dot_pos - 1)
            src->exponent++;
    }    
}

void dump_print(const bigfloat_t src)
{
    puts(ANSI_COLOR_YELLOW "\n------DUMP PRINT STARTS------" RESET_COLOR);
    printf("SIGN: %d\n", src.mant_sign); //  == POSITIVE ? "+" : "-"
    printf("DOT POS: %d\n", src.dot_pos);
    printf("MANT LEN: %d\n", src.mant_len);
    printf("NUMBER: ");

    for (int i = 0; i < src.mant_len; i++)
    {
        if (i == src.dot_pos)
            printf(".");
        printf("%d", src.mantissa[i]);
    }
    printf(" E %d\n", src.exponent);
    puts(ANSI_COLOR_YELLOW "------DUMP PRINT ENDS--------" RESET_COLOR);
}

void normalise(bigfloat_t *src)
{
    if (is_nil(*src))
        return;
    if (src->dot_pos != MANTISSA_LENGTH)
    {
        int i = 0;
        while (!src->mantissa[i])
        {
            i++;
        }

        if (i > src->dot_pos && src->mantissa[i])
        {
            src->exponent -= (i);
        }

        for (int j = 0; j < i; j++)
        {
            mantissa_shift_left(src, 1);
            src->mant_len--;
        }
        src->exponent += src->dot_pos;
    }
    else
    {
        int i = src->mant_len - 1;

        while (!src->mantissa[i])
        {
            src->exponent++;
            src->mant_len--;
            i--;
        }
    
        src->exponent += src->mant_len;
    }
    
}

int is_nil(const bigfloat_t num)
{
    for (int i = 0; i < MANTISSA_LENGTH; i++)
        if (num.mantissa[i])
            return FALSE;
    
    return TRUE;
}

my_error_t mant_sub(bigfloat_t *divided, bigfloat_t *diviser)
{
    if (is_greater(divided, diviser))
    {
        for (int base = MANTISSA_LENGTH - 1; base >= 0; --base)
        {
            if (divided->mantissa[base] - diviser->mantissa[base] >= 0)
            {
                divided->mantissa[base] -= diviser->mantissa[base];
            } 
            else
            {
                int new_base = base - 1;

                while (divided->mantissa[new_base] == 0)
                    new_base--;

                divided->mantissa[new_base]--;
                new_base++;


                for (; new_base < base; new_base++)
                {
                    divided->mantissa[new_base] += 9;
                }

                divided->mantissa[base] += (10 - diviser->mantissa[base]);

            }
        }

        return NO_ERROR;
    }

    return SUB_ERROR;
}

int count_div_iters(bigfloat_t *divided, bigfloat_t *diviser)
{
    int iters = 0;

    while (mant_sub(divided, diviser) == NO_ERROR)
    {
        iters++;
    }

    mant_offset(divided, 1);

    return iters;
}

my_error_t division(bigfloat_t *divided, bigfloat_t *diviser, bigfloat_t *res)
{
    if (is_nil(*diviser))
        return DIVISION_BY_ZERO;
    
    if (is_nil(*divided))
    {
        res->dot_pos = 0;
        res->exponent = 0;
        res->mant_len = 1;
        res->mant_sign = POSITIVE;
        res->mantissa[0] = 0;
        return NO_ERROR;
    }

    if (abs(divided->exponent - diviser->exponent) > 99999)
        return OVERFLOW_EXP;
    
    if (!is_greater(divided, diviser))
    {
        divided->exponent--;
        mant_offset(divided, 1);
    }    

    res->mantissa[0] = 0;
    int temp = count_div_iters(divided, diviser);
    res->mantissa[1] = temp;

    int iters;

    for (iters = 2; iters < MANTISSA_LENGTH - 2; ++iters)
    {
        if (is_nil(*divided))
        {
            res->mant_len = iters;
            temp = 0;
            break;
        }

        if (!is_greater(divided, diviser))
        {
            mant_offset(divided, 1);
            res->mantissa[iters] = 0;
        }
        else
        {
            temp = count_div_iters(divided, diviser);
            res->mantissa[iters] = temp;
        }
    }

    res->mant_len = iters;

    if ((iters == MANTISSA_LENGTH - 2) && (temp = count_div_iters(divided, diviser) >= 5))
        res->mantissa[MANTISSA_LENGTH - 3]++;
    
    for (; iters < MANTISSA_LENGTH - 2; ++iters)
        res->mantissa[iters] = 0;
    
    int inc = FALSE;

    for (iters = MANTISSA_LENGTH - 3; iters >= 0; --iters)
    {
        res->mantissa[iters] += inc;
        
        if (res->mantissa[iters] == 10)
        {
            res->mantissa[iters] = 0;
            inc = TRUE;
        }
        else
            break;
    }

    if (res->mantissa[0] == 1)
    {
        for (iters = MANTISSA_LENGTH - 2; iters > 0; --iters)
            res->mantissa[iters] = res->mantissa[iters - 1];
        res->mantissa[0] = 0;
    }
    else
        inc = FALSE;
    
    res->mant_sign = !(divided->mant_sign ^ diviser->mant_sign);

    res->exponent = divided->exponent - diviser->exponent + inc + 1;

    if (res->exponent > 99999)
        return OVERFLOW_EXP;
    
    // remove_last_zeroes(res);

    return NO_ERROR;
}
