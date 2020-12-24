#include "error.h"

char *mystrerr(int ernum)
{
    switch(ernum)
    {
    case ALLOCATION_FAILED:
        return ALLOCATION_FAILED_MSG;
    
    case BAD_ROW:
        return BAD_ROW_MSG;
    
    case BAD_CLM:
        return BAD_CLM_MSG;
    
    case EMPTY_MATRIX:
        return EMPTY_MATRIX_MSG;
    
    case BAD_PERCENT:
        return BAD_PERCENT_MSG;
    
    case BAD_VEC_ELEM:
        return BAD_VEC_ELEM_MSG;
    
    case BAD_VEC_LEN:
        return BAD_VEC_LEN_MSG;
    
    case EMPTY_VECTOR:
        return EMPTY_VECTOR_MSG;
    
    case ROWS_NOT_EQUAL_COLUMNS:
        return ROWS_NOT_EQUAL_COLUMNS_MSG;
    
    case EMPTY_RES:
        return EMPTY_RES_MSG;
    
    case NOT_ENOUGH_OPERANDS:
        return NOT_ENOUGH_OPERANDS_MSG;
    
    case EMPTY_MULT:
        return EMPTY_MULT_MSG;

    default:
        return "No such process";
    }
}