#include "error.h"

char *mystrerr(int errnum)
{
    switch (errnum)
    {
        case ALLOC_FAILED:
            return ALLOC_FAILED_MSG;
        case BAD_SEQ_LEN:
            return BAD_SEQ_LEN_MSG;
        case EMPTY_STACK:
            return EMPTY_STACK_MSG;
        case BAD_ELEM:
            return BAD_ELEM_MSG;
        case NOT_INPUTTED:
            return NOT_INPUTTED_MSG;
        case UNEXISTING_FILE:
            return UNEXISTING_FILE_MSG;
        case BAD_FILE:
            return BAD_FILE_MSG;
        case EMPTY_FILE:
            return EMPTY_FILE_MSG;
        case STACK_OVERFLOW:
            return STACK_OVERFLOW_MSG;
        case TOO_BIG_LEN:
            return TOO_BIG_LEN_MSG;
        default:
            return "no such process";
    }
}
