#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "io.h"
#include "logger.h"
#include "array_stack.h"
#include "list_stack.h"
#include "stack.h"
#include "timer.h"

int main(void)
{
    print_menu();
    int menu;

    arr_stack_t *arr_stack = NULL;
    list_stack_t *list_stack = NULL;

    START_MEASURING();

    while (1)
    {
        int res = 0xff;

        printf("\nВыберите пункт меню: ");
        fpurge(stdin);
        
        if (scanf("%d", &menu) != 1)
            menu = UKNOWN;

        switch (menu)
        {
            case INPUT_SEQUENCE:
                res = input_sequence(&arr_stack, &list_stack);
                break;
            case PRINT_STACK_ARRAY:
                if (arr_stack == NULL)
                    res = EMPTY_STACK;
                else
                    res = PRINT(*arr_stack);
                break;
            case ADD_ELEM_ARRAY:
                res = ADD(&arr_stack);
                break;
            case PRINT_STACK_LIST:
                if (list_stack == NULL)
                    res = EMPTY_STACK;
                else
                    res = PRINT(*list_stack);
                break;
            case ADD_ELEM_LIST:
                res = ADD(&list_stack);
                break;
            case FIND_SEQUENCES:
                res = find_seq(arr_stack, list_stack);
                break;
            case PRINT_HELP:
                print_menu();
                res = NO_ERROR;
                break;
            case CLEAR_SCREEN:
                system("clear");
                res = NO_ERROR;
                break;
            case READ_FROM_FILE:
                res = read_seq_from_file(&arr_stack, &list_stack);
                break;
            case EXIT:
                puts("Прощайте.");
                goto clear;
            default:
                PRINT_ERROR("%s", "Неизвестная команда.\n");
                print_menu();
                res = NO_ERROR;
        }

        if (res == ALLOC_FAILED)
        {
            puts("Allocation failed!");
            return ALLOC_FAILED;
        }

        if (res)
            PRINT_ERROR("%s", mystrerr(res));

    }

clear:
    CLEAR(&arr_stack);
    CLEAR(&list_stack);

    END_MEASURING();

    SHOW_TIME("Время исполнения: ");

    return NO_ERROR;
}
