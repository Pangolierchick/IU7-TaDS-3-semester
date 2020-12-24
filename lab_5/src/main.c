#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "lqueue.h"
#include "errors.h"
#include "lmodel.h"
#include "amodel.h"
#include "defines.h"
#include "menu.h"
#include "colors.h"
#include "timer.h"
#include "aqueue.h"
#include "lqueue.h"

#define MAX(a, b) ((a) < (b) ? b : a)
#define MIN(a, b) ((a) > (b) ? b : a)

#define MODEL_SUPPOSED_WAITING MAX((time_res.total_in - 1000) * T2_WAITING_MIDDLE, 1000 * T1_WAITING_MIDDLE)
#define MODEL_SUPPOSED_SERVICE MAX((time_res.coll - 1000) * T2_WAITING_MIDDLE, 1000 * T1_WAITING_MIDDLE)
#define MODEL_SUPPOSED_TIME    (MAX(MODEL_SUPPOSED_WAITING, MODEL_SUPPOSED_SERVICE))

#define PRINT_RES(time_res)                                                                                                     \
                printf(YELLOW_MSG("\nРезультаты\n"));                                                                           \
                printf("Время обслуживания: %lf\tВремя простаивания: %lf\n", time_res.service_time, time_res.waiting_time);     \
                printf("Время моделирования: %lf\n", MAX(MAX(time_res.t1_entrance_time, time_res.t2_entrance_time), time_res.t1_service_time + time_res.t2_service_time));                \
                printf("Ожидаемое время моделирования: %lf (%lf%%)\n", MODEL_SUPPOSED_TIME, (fabs(MODEL_SUPPOSED_TIME - time_res.service_time + time_res.waiting_time) / (time_res.service_time + time_res.waiting_time)));\
                printf("Кол-во вошедших заявок: %d\n", time_res.total_in);                                                      \
                printf("Кол-во вышедших заявок: %d\n", time_res.coll);                                                          \
                printf("Аппарат сработал: %d\n", time_res.oa_calls);                                                            \


int print_pointers;
struct timespec start_time_s, end_time_s;

int main(void)
{
    tm_res time_res = { 0 };
    int menu_p = 0xfffff;
    int res;

    while (menu_p)
    {
        print_menu();

        printf("Выберите пункт меню: ");
        fpurge(stdin);
        if (scanf("%d", &menu_p) != 1)
        {
            printf(RED_COLOR "Ошибка" RESET_COLOR ": Неправильный ввод\n");
            continue;
        }

        switch (menu_p)
        {
        case LIST_MODEL:
            ask_y_n("Выводить адреса при удалении/добавлении? (y/n): ", print_pointers);
            memset(&time_res, 0, sizeof(tm_res));

            START_MEASURING();
            res = l_queue_model(&time_res);
            END_MEASURING();

            PRINT_RES(time_res);

            SHOW_TIME("Время выполнения: ");
            printf(YELLOW_MSG("Потребление памяти: "));
            printf("%luБ\n", time_res.coll * (sizeof(data_t) + sizeof(queue_node_t)));

            break;

        case ARRAY_MODEL:
            ask_y_n("Выводить адреса при удалении/добавлении? (y/n): ", print_pointers);
            memset(&time_res, 0, sizeof(tm_res));

            START_MEASURING();
            res = a_queue_model(&time_res);
            END_MEASURING();

            PRINT_RES(time_res);

            SHOW_TIME("Время выполнения: ");
            printf(YELLOW_MSG("Потребление памяти: "));
            printf("%luБ\n", time_res.coll * sizeof(data_t));

            break;

        case OPERATION_COMP:
            puts("\nСравнение времени");
            uint64_t start = 0;
            uint64_t end = 0;
            uint64_t time_res = 0;
            const int iter = 50;

            aqueue_t* aq = init_aqueue();
            lqueue_t* lq = init_lqueue();

            data_t* ptmp = malloc(sizeof(data_t));
            ptmp->entrance = 4;
            ptmp->last_entrance = 3;
            ptmp->last_service = 3;
            ptmp->type = 1;

            start = tick();
            for (int i = 0; i < iter; i++)
                add_aqueue(aq, *ptmp);
            end = tick();

            printf(COLOR_YELLOW "Добавление в массиве" RESET_COLOR ": %llu\n", (end - start) / iter);

            start = tick();
            for (int i = 0; i < iter; i++)
                add_lqueue(lq, ptmp);
            end = tick();

            printf(COLOR_YELLOW "Добавление в списке" RESET_COLOR ": %llu\n", (end - start) / iter);

            data_t* dummy = malloc(sizeof(data_t));

            for (int i = 0; i < iter; i++)
            {

                start = tick();
                pop_aqueue(aq, dummy);
                end = tick();

                time_res += end - start;

                add_aqueue(aq, *dummy);
            }

            printf(COLOR_YELLOW "Удаление в массиве" RESET_COLOR ": %llu\n", (time_res) / iter);

            start = tick();
            for (int i = 0; i < iter; i++)
                pop_lqueue(lq);
            end = tick();

            printf(COLOR_YELLOW "Удаление в списке" RESET_COLOR ": %llu\n", (end - start) / iter);

            full_clean_aqueue(&aq);
            clean_lqueue(&lq);
            free(ptmp);
            free(dummy);

            res = 0;

            break;

        case 0:
            break;

        default:
            printf(RED_COLOR "Ошибка" RESET_COLOR ": Такого пунтка меню нет.\n");
            break;
        }
    }

    return res;
}
