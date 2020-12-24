#include <string.h>
#include <stdio.h>
#include "error.h"
#include "io.h"
#include "static_vector.h"
#include "funcs.h"


int main(void)
{
    static_vector_t table;
    init_vector(&table);

    print_hello();

    const int cmd_len = 11;

    char cmd[cmd_len];

    int res = 0;

    // MAIN MENU
    while (1)
    {
        ASK_CMD();
        fpurge(stdin);
        scanf("%10s", cmd);

        if (!strcmp(cmd, PRINT_CMD))
        {
            print_table(table);
        }
        else if (!strcmp(cmd, ADD_CMD))
        {
            res = add_record(&table);
        }
        else if (!strcmp(cmd, DELETE_CMD))
        {
            res = delete_record(&table);
        }
        else if (!strcmp(cmd, BUBBLE_SORT_CMD))
        {
            START_SORT_MSG("ПУЗЫРЕК");
            int is_key = ask_y_n("Сортировать по таблице ключей? (y/n): ");

            if (is_key == BAD_YES_NO)
                res = BAD_YES_NO;
            else
                init_sort(&table, BUBBLE_SORT, is_key);

        }
        else if (!strcmp(cmd, QSORT_SORT_CMD))
        {
            START_SORT_MSG("БЫСТРАЯ СОРТИРОВКА");
            int is_key = ask_y_n("Сортировать по таблице ключей? (y/n): ");
    
            if (is_key == BAD_YES_NO)
                res = BAD_YES_NO;
            else
                init_sort(&table, QUICK_SORT, is_key);

        }
        else if (!strcmp(cmd, FIND_TASK_CMD))
        {
            res = find_task(&table);
        }
        else if (!strcmp(cmd, DUMP_TABLE_CMD))
        {
            res = write_in_file(&table);
        }
        else if (!strcmp(cmd, LOAD_TABLE_CMD))
        {
            res = read_from_file(&table);
        }
        else if (!strcmp(cmd, EXIT_CND))
        {
            system("figlet -f slant 'goodbye'");
            break;
        }
        else if (!strcmp(cmd, CLEAR_CMD))
        {
            system("clear");
        }
        else
        {
            puts(RED_MSG("Введена неизвестная команда!"));
            print_help();
        }

        if (res)
        {
            printf("\033[0;31mОшибка\033[0m: %s\n", mystrerr(res));
            res = 0;
        }
    }

    return NO_ERROR;
}