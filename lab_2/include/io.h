#ifndef __IO_H__
#define __IO_H__

#define DEBUG               0


#if DEBUG
#pragma message "DEBUG MODE IS ON"
#endif

#define COLOR_YELLOW            "\x1b[33m"
#define RED_COLOR               "\033[0;31m"
#define RESET_COLOR             "\033[0m"

#define RED_MSG(msg) (RED_COLOR msg RESET_COLOR)



#define FILE_END            50

#define ASK_CMD() (printf("Введите комманду: "))
#define START_SORT_MSG(type) (printf("Начинаем сортировку <<%s>>. Сортировка будет проводится по цене в порядке возрастания.\n", type))

#include "defines.h"
#include "static_vector.h"

#define PRINT_CMD               "print"
#define ADD_CMD                 "add"
#define DELETE_CMD              "del"
#define BUBBLE_SORT_CMD         "bsort"
#define QSORT_SORT_CMD          "qsort"
#define FIND_TASK_CMD           "task"
#define DUMP_TABLE_CMD          "dump"
#define LOAD_TABLE_CMD          "load"
#define CLEAR_CMD               "clear"
#define EXIT_CND                "exit"

#define SORT_TYPE_TABLE         "table"
#define SORT_TYPE_KEYS          "keys"



int ask_y_n(const char *msg);
void print_help(void);
void print_hello(void);

int read_one_car(vector_type *car);

void print_table(const static_vector_t table);

int add_record(static_vector_t *table);
int delete_record(static_vector_t *table);

int write_in_file(static_vector_t *table);
int write_rec_in_file(FILE *f, vector_type *record);

int read_from_file(static_vector_t *table);
int read_car_from_file(FILE *f, vector_type *record);

#endif // __IO_H__