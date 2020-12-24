#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include "list_stack.h"
#include "error.h"

int list_err;

#define ADD_NODE(node, val)                         \
    node_t *new_node = calloc(1, sizeof(node_t));   \
    if (new_node == NULL)                           \
        return ALLOC_FAILED;                        \
    node->next = new_node;                          \
    new_node->val = val;                            \

list_stack_t *list_init_stack(void)
{
    list_stack_t *stack = malloc(sizeof(list_stack_t));

    if (stack == NULL)
        return NULL;

    stack->head = NULL;
    stack->len = 0;

    INFO_PRINT("Stack inited %p. Mem alloced: %ld\n", GENERICK_POINTER(stack), sizeof(list_stack_t));

    return stack;
}

void list_clear_stack(list_stack_t **stack)
{
    if (*stack)
    {
        node_t *curr_node = (*stack)->head;
        node_t *next_node = NULL;

        int i = 0;

        while (curr_node != NULL)
        {
            next_node = curr_node->next;
            free(curr_node);
            curr_node = next_node;
            i++;
        }

        free(*stack);

        *stack = NULL;

        INFO_PRINT("Clearing %ld memory\n", sizeof(list_stack_t) + i * sizeof(node_t));
    }
}

// [1] -> [2] -> [3]
// [1]

int list_pop(list_stack_t *stack)
{
    node_t *curr_node = stack->head;
    node_t *last_node = NULL;
    
    if (curr_node == NULL)
    {
        list_err = STACK_UNDERFLOW;
        return -1;
    }

    if (curr_node->next == NULL)
    {
        int val = curr_node->val;
        stack->head = NULL;
        free(curr_node);
        return val;
    }
    
    while (curr_node->next != NULL)
    {
        last_node = curr_node;
        curr_node = curr_node->next;
    }

    last_node->next = NULL;

    int val = curr_node->val;

    DBG_PRINT("Poppin %d\n", val);

    free(curr_node);

    stack->len--;

    return val;
}

int list_append(list_stack_t *stack, int val)
{
    TRACE_PRINT("Appending %d value (%d:%d)\n", val, stack->len, LIST_MAX_LEN);

    if (stack->len > LIST_MAX_LEN)
    {
        ERROR_PRINT("Stack overflow.\n");
        return STACK_OVERFLOW;
    }

    node_t *curr_node = stack->head;

    if (curr_node == NULL)
    {
        node_t *new_node = calloc(1, sizeof(node_t));
        if (new_node == NULL)
        {
            ERROR_PRINT("Alloc failed.");
            return ALLOC_FAILED;
        }

        stack->head = new_node;
        stack->head->val = val;
        return NO_ERROR;    
    }

    while (curr_node->next != NULL)
        curr_node = curr_node->next;

    ADD_NODE(curr_node, val);

    stack->len++;

    return NO_ERROR;
}

int list_print_stack(list_stack_t stack)
{
    node_t *curr_node = stack.head;

    if (curr_node == NULL)
        return EMPTY_STACK;

    printf("HEAD ");

    while (curr_node)
    {
        printf("%d (%p)", curr_node->val, GENERICK_POINTER(curr_node));
        curr_node = curr_node->next;

        if (curr_node)
            printf(" -> ");
    }

    printf(" TAIL");

    return NO_ERROR;
}

list_stack_t *lstack_cpy(list_stack_t *src)
{
    list_stack_t *dst = list_init_stack();

    if (dst == NULL)
        return dst;
    
    for (node_t *cnode = src->head; cnode != NULL; cnode = cnode->next)
    {
        if (list_append(dst, cnode->val) == ALLOC_FAILED)
        {
            list_clear_stack(&dst);
            return NULL;
        }

    }
    
    return dst;
}

int get_len(list_stack_t *src)
{
    node_t *curr_node = src->head;
    int len = 0;

    while (curr_node != NULL)
    {
        curr_node = curr_node->next;
        len++;
    }
    
    return len;
}
