#include "error.h"
#include "static_vector.h"

char *mystrerr(int errnum)
{
    switch (errnum)
    {
        case BAD_BRAND:
            return BAD_BRAND_MSG;
        case BAD_COUNTRY:
            return BAD_COUNTRY_MSG;
        case BAD_COST:
            return BAD_COST_MSG;
        case BAD_COLOR:
            return BAD_COLOR_MSG;
        case BAD_YES_NO:
            return BAD_YES_NO_MSG;
        case BAD_WARRANTY:
            return BAD_WARRANTY_MSG;
        case BAD_PROD_YEAR:
            return BAD_PROD_YEAR_MSG;
        case BAD_MILEAGE:
            return BAD_MILEAGE_MSG;
        case BAD_REPAIR_NUM:
            return BAD_REPAIR_NUM_MSG;
        case BAD_OWNER_NUM:
            return BAD_OWNER_NUM_MSG;
        case BAD_RECORD_NUM:
            return BAD_RECORD_NUM_MSG;
        case BAD_FILENAME:
            return BAD_FILENAME_MSG;
        case UNEXISTING_FILE:
            return UNEXISTING_FILE_MSG;
        case BAD_FILE:
            return BAD_FILE_MSG;
        case INDEX_OUT_OF_BOUNDS:
            return INDEX_OUT_OF_BOUNDS_MSG;
        case VECTOR_UNDERFLOW:
            return VECTOR_UNDERFLOW_MSG;
        case VECTOR_OVERFLOW:
            return VECTOR_OVERFLOW_MSG;
        default:
            return "No such process";
    }
}