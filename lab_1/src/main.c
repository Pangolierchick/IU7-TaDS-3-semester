#include <stdio.h>
#include <string.h>

#include "include/myio.h"
#include "include/error.h"
#include "include/bigfloat.h"

int main(void)
{
    hello_print();
    bigfloat_t long_int;
    bigfloat_t long_float;
    memset(&long_int, 0, sizeof(bigfloat_t));
    memset(&long_float, 0, sizeof(bigfloat_t));
    
    int bf_err = input_bigfloat(&long_float);

    if (bf_err)
    {
        fprintf(stderr, "%s\n", mystrerr(bf_err));
        return bf_err;
    }

    int bi_err = input_long_int(&long_int);

    if (bi_err)
    {
        fprintf(stderr, "%s\n", mystrerr(bi_err));
        return bi_err;
    }

    
    normalise(&long_int);
    normalise(&long_float);

    // dump_print(long_int);
    // dump_print(long_float);


    remove_lead_zeros(&long_int);
    remove_lead_zeros(&long_float);


    right_shift(&long_int);
    right_shift(&long_float);


    bigfloat_t res;
    memset(&res, 0, sizeof(bigfloat_t));


    int res_err = division(&long_float, &long_int, &res);

    if (res_err)
    {
        fprintf(stderr, "%s\n", mystrerr(res_err));
        return res_err;
    }

    print_res(res);

    return NO_ERROR;
}