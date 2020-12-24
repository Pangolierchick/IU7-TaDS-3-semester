#include <stdlib.h>
#include <string.h>
#include "array_stack.h"
#include "logger.h"
#include "error.h"

int arr_err;

arr_stack_t *arr_init_stack(void)
{
    arr_stack_t *stack = calloc(1, sizeof(arr_stack_t));

    if (stack == NULL)
        return NULL;
    
    int act_len = INITIAL_CAPACITY;

    stack->arr = malloc(act_len * sizeof(int));

    if (stack->arr == NULL)
    {
        free(stack);
        return NULL;
    }

    stack->capacity = act_len;
    stack->len = 0;

    INFO_PRINT("Stack %p inited. Mem. alloced: %ld\n", GENERICK_POINTER(stack), sizeof(arr_stack_t) + sizeof(int) * act_len);

    return stack;
}

void arr_clear_stack(arr_stack_t **stack)
{
    if (*stack)
    {
        INFO_PRINT("Clearing %ld memory.\n", sizeof(arr_stack_t) + sizeof(int) * (*stack)->capacity);

        free((*stack)->arr);
        free(*stack);

        *stack = NULL;

    }
}

int arr_pop(arr_stack_t *stack)
{
    DBG_PRINT("Poppin %d:%d\n", stack->len, stack->capacity);
    if (!stack->len)
    {
        DBG_PUTS("Underflow.");
        arr_err = STACK_UNDERFLOW;
        return -1;
    }

    return stack->arr[--stack->len];
}

int arr_append(arr_stack_t *stack, int val)
{
    TRACE_PRINT("Appending %d val in stack %d:%d\n", val, stack->len, stack->capacity);

    if (stack->len >= stack->capacity)
    {
        /*
        stack->capacity *= 2;
        stack->arr = realloc(stack->arr, sizeof(int) * stack->capacity);

        INFO_PRINT("Stack realloced. Mem usage: %ld\n", sizeof(int) * stack->capacity);
        
        if (stack->arr == NULL)
            return ALLOC_FAILED;
        */

       ERROR_PRINT("Stack overflow.\n");

       return STACK_OVERFLOW;
    }

    stack->arr[stack->len++] = val;

    return NO_ERROR;
}

int arr_print_stack(arr_stack_t stack)
{
    if (!stack.len)
        return EMPTY_STACK;

    printf("\nLen %d\n", stack.len);

    printf("HEAD ");
    for (int i = 0; i < stack.len; i++)
    {
        printf("%d", stack.arr[i]);

        if (i < stack.len - 1)
            printf(" -> ");
    }
    printf(" TAIL\n");

    return NO_ERROR;
}

arr_stack_t *astack_cpy(arr_stack_t *src)
{
    arr_stack_t *dst = arr_init_stack();
    
    if (dst == NULL)
        return NULL;

    memcpy(dst->arr, src->arr, src->len * sizeof(int));
    dst->len = src->len;

    return dst;
}
