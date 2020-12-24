#ifndef __LIST_STACK_H__
#define __LIST_STACK_H__

#define LIST_MAX_LEN                5000

extern int list_err;

typedef struct node_s
{
    int val;
    struct node_s *next;
} node_t;

typedef struct list_stack_s
{
    node_t *head;
    int len;
} list_stack_t;

int list_pop(list_stack_t *stack);
int list_append(list_stack_t *stack, int val);
int list_print_stack(list_stack_t stack);

list_stack_t *list_init_stack(void);
void list_clear_stack(list_stack_t **stack);
list_stack_t *lstack_cpy(list_stack_t *src);
int get_len(list_stack_t *src);

#endif // __LIST_STACK_H__
