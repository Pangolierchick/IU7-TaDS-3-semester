#ifndef  __ARRAY_STACK_H__
#define __ARRAY_STACK_H__

#define INITIAL_CAPACITY            100

extern int arr_err;

typedef struct arr_stack_s
{
    int *arr;
    int len;
    int capacity;
} arr_stack_t;

int arr_pop(arr_stack_t *stack);
int arr_look(arr_stack_t stack);
int arr_append(arr_stack_t *stack, int val);
int arr_print_stack(arr_stack_t stack);

arr_stack_t *arr_init_stack(void);
void arr_clear_stack(arr_stack_t **stack);
arr_stack_t *astack_cpy(arr_stack_t *src);

#endif // __ARRAY_STACK_H__
