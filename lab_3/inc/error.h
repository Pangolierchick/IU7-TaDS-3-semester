#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdio.h>

#define DEBUG               0
#define INFO                0

#if DEBUG == 1
#pragma message "Debug mode is on"
#define DBG_PUTS(msg)          fprintf(stderr, "[DBG] [%s] [%15s:%4d] %s\n" , __FILE__, __func__, __LINE__, msg)
#define DBG_PUT(msg)           fprintf(stderr, "[DBG] [%s] [%15s:%4d] %s" , __FILE__, __func__, __LINE__, msg)
#define DBG_PRINT(format, ...) fprintf(stderr, "[DBG] [%s] [%15s:%4d] " format, __FILE__, __func__, __LINE__, __VA_ARGS__)
#else
#define DBG_PUTS(msg)
#define DBG_PRINT(format, ...)
#endif

#if INFO == 1
#define INFO_PUTS(msg)           fprintf(stderr, "[INFO] [%19s] [%17s:%4d] %s\n" , __FILE__, __func__, __LINE__, msg)
#define INFO_PUT(msg)            fprintf(stderr, "[INFO] [%19s] [%17s:%4d] %s" , __FILE__, __func__, __LINE__, msg)
#define INFO_PRINT(format, ...)  fprintf(stderr, "[INFO] [%19s] [%17s:%4d] " format, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define INFO_PRINTF(format, ...) fprintf(stderr, format, __VA_ARGS__);
#else
#define INFO_PUTS(msg)
#define INFO_PRINT(format, ...)
#define INFO_PUT(msg)
#define INFO_PRINTF(format, ...)
#endif




#define PRINT_ERROR(format, ...)        printf(RED_COLOR "Ошибка: " RESET_COLOR format "\n", __VA_ARGS__)

#define NO_ERROR                            0
#define ALLOCATION_FAILED                   10
#define BAD_ROW                             11
#define BAD_CLM                             12
#define EMPTY_MATRIX                        13
#define BAD_PERCENT                         14
#define BAD_VEC_ELEM                        15
#define BAD_VEC_LEN                         16
#define EMPTY_VECTOR                        17
#define ROWS_NOT_EQUAL_COLUMNS              18
#define EMPTY_RES                           19
#define NOT_ENOUGH_OPERANDS                 20
#define EMPTY_MULT                          21

#define ALLOCATION_FAILED_MSG               "Не удалось выделить память."
#define BAD_ROW_MSG                         "Неправильно введен ряд матрицы."
#define BAD_CLM_MSG                         "Неправильно введен столбец матрицы."
#define EMPTY_MATRIX_MSG                    "Пустая матрица."
#define BAD_PERCENT_MSG                     "Неправильно введен процент."
#define BAD_VEC_ELEM_MSG                    "Неправильно введен элемент вектора."
#define BAD_VEC_LEN_MSG                     "Неправильно введена длина вектора."
#define EMPTY_VECTOR_MSG                    "Пустой вектор."
#define ROWS_NOT_EQUAL_COLUMNS_MSG          "Длина вектора не равна кол-ву столбцов матрицы. Умножение невозможно."
#define EMPTY_RES_MSG                       "Умножение еще не проводилось."
#define NOT_ENOUGH_OPERANDS_MSG             "Недостаточно операндов умножения. Не были введены матрица или вектор."
#define EMPTY_MULT_MSG                      "Результат умножения пуст."

char *mystrerr(int ernum);

#endif // __ERROR_H__
