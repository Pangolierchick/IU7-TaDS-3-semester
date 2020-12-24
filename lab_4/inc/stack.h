#ifndef __STACK_H__
#define __STACK_H__

#include "array_stack.h"
#include "list_stack.h"

#define POP(stack) _Generic((stack),                     \
                   arr_stack_t*: arr_pop,                 \
                   list_stack_t*: list_pop                \
)(stack)


#define CLEAR(stack) _Generic((stack),                   \
                   arr_stack_t**: arr_clear_stack,       \
                   list_stack_t**: list_clear_stack      \
)(stack)


#define APPEND(stack, elem) _Generic((stack),            \
                   arr_stack_t*: arr_append,             \
                   list_stack_t*: list_append            \
)(stack, elem)


#define PRINT(stack) _Generic((stack),                   \
                   arr_stack_t: arr_print_stack,         \
                   list_stack_t: list_print_stack        \
)(stack)

#define CPY(stack) _Generic((stack),                     \
                   arr_stack_t*: astack_cpy,             \
                   list_stack_t*: lstack_cpy             \
)(stack)

#define ADD(stack) _Generic((stack),                        \
                   arr_stack_t**: add_elem_arr,              \
                   list_stack_t**: add_elem_list             \
)(stack)



#endif // __STACK_H__
