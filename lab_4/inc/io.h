#ifndef __IO_H__
#define __IO_H__

#include "array_stack.h"
#include "list_stack.h"

typedef enum menu_e
{
    EXIT,
    INPUT_SEQUENCE,
    PRINT_STACK_ARRAY,
    ADD_ELEM_ARRAY,
    PRINT_STACK_LIST,
    ADD_ELEM_LIST,
    FIND_SEQUENCES,
    PRINT_HELP,
    CLEAR_SCREEN,
    READ_FROM_FILE,
    UKNOWN,
} menu_t;

void print_menu(void);

int input_sequence(arr_stack_t **astack, list_stack_t **lstack);
int find_seq(arr_stack_t *astack, list_stack_t *lstack);
int read_seq_from_file(arr_stack_t **astack, list_stack_t **lstack);
int add_elem_arr(arr_stack_t **arr);
int add_elem_list(list_stack_t **list);

#endif // __IO_H__
