#ifndef __ERROR_H__
#define __ERROR_H__

#include "colors.h"

#define NO_ERROR                        0
#define ALLOC_FAILED                    1
#define BAD_SEQ_LEN                     2
#define STACK_UNDERFLOW                 3
#define EMPTY_STACK                     4
#define BAD_ELEM                        5
#define NOT_INPUTTED                    6
#define UNEXISTING_FILE                 7
#define BAD_FILE                        8
#define EMPTY_FILE                      9
#define STACK_OVERFLOW                 10
#define TOO_BIG_LEN                    11

#define PRINT_ERROR(format, ...)        printf(RED_COLOR "Ошибка: " RESET_COLOR format "\n", __VA_ARGS__)


#define ALLOC_FAILED_MSG                "Не удалось выделить память."
#define BAD_SEQ_LEN_MSG                 "Неправильно введена длина последовательности."
#define EMPTY_STACK_MSG                 "Стек пуст."
#define BAD_ELEM_MSG                    "Неправильно введен элемент последовательности."
#define NOT_INPUTTED_MSG                "Последовательность еще не была введена."
#define UNEXISTING_FILE_MSG             "Такой файл не существует."
#define BAD_FILE_MSG                    "Файл содержит некорректные данные."
#define EMPTY_FILE_MSG                  "Файл пуст."
#define STACK_OVERFLOW_MSG              "Стек полон, добавление невозможно."
#define TOO_BIG_LEN_MSG                 "Введена слишком большая длина последовательности."

char *mystrerr(int errnum);


#endif // __ERROR_H__
