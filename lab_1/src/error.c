#include "include/error.h"

char *mystrerr(my_error_t err)
{
    switch (err)
    {
        case DIVISION_BY_ZERO:
            return DIVISION_BY_ZERO_MSG();
        case OVERFLOW_MANT:
            return OVERFLOW_MANT_MSG();
        case OVERFLOW_EXP:
            return OVERFLOW_EXP_MSG();
        case INCORRECT_E:
            return INCORRECT_E_MSG();
        case INCORRECT_SIGN:
            return INCORRECT_SIGN_MSG();
        case INCORRECT_DIGIT:
            return INCORRECT_DIGIT_MSG();
        case SINGLETON_DOT:
            return SINGLETON_DOT_MSG();
        case TOO_LONG_MANT:
            return TOO_LONG_MANT_MSG();
        case TOO_LONG_EXP:
            return TOO_LONG_EXP_MSG();
        case SIGN_AFTER_E:
            return SIGN_AFTER_E_MSG();
        case ZERO_LEN_MANT:
            return ZERO_LEN_MANT_MSG();
        case ZERO_LEN_EXP:
            return ZERO_LEN_EXP_MSG();
        case TOO_LONG_BIGINT:
            return TOO_LONG_BIGINT_MSG();
        case ZERO_LEN_BIGINT:
            return ZERO_LEN_BIGINT_MSG();
        default:
            return "No such process";
    }
}