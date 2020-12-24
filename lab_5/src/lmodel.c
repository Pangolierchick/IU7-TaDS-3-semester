#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "logger.h"
#include "lmodel.h"
#include "errors.h"

#define ENTRANCE_TIME(lqueue) (((data_t *)(lqueue)->head->data)->last_entrance)
#define SERVICE_TIME(lqueue)  (((data_t *)(lqueue)->head->data)->last_service)

#define NEEDED_ENTRANCE_TIME(lqueue) (((data_t *)(lqueue)->head->data)->entrance)
#define NEEDED_SERVICE_TIME(lqueue)  (((data_t *)(lqueue)->head->data)->service)


static int type_cmp(void *a, void *b)
{
    data_t *pa = (data_t *) a;
    queue_type_t pb = *((queue_type_t *) b);

    return pb == pa->type;
}

#define L_INIT_NODE(__node, __type)                                                     \
    do                                                                                  \
    {                                                                                   \
        __node->entrance = lfrnd(__type##_WAITING_TIME_MIN, __type##_WAITING_TIME_MAX); \
        __node->service = lfrnd(__type##_SERVICE_TIME_MIN, __type##_SERVICE_TIME_MAX);  \
        __node->last_entrance = 0;                                                      \
        __node->last_service = 0;                                                       \
        __node->type = __type;                                                          \
    } while (0);

static void l_increase_time(lqueue_t *q, int type)
{
    queue_node_t *i = q->head;

    if (!i)
        return;

    if (type == INDIVIDUAL_QUEUE)
        ((data_t *)i->data)->last_entrance += TIME_PER_LOOP;
    else
        ((data_t *)i->data)->last_service += TIME_PER_LOOP;
}

int l_queue_model(tm_res *res)
{
    lqueue_t *q1 = init_lqueue();
    lqueue_t *q2 = init_lqueue();
    lqueue_t *coll = init_lqueue();

    srand(time(NULL));

    if (!(q1 && q2 && coll))
    {
        clean_lqueues(3, &q1, &q2, &coll);

        ERROR_PRINT("Alloc error\n");

        return ALLOC_ERROR;
    }

    int q1_elems = 0;
    int last_q1 = 0;

    int add_res;
    int add_coll_res;
    int pop_coll_res;

    int q1_len = 0;
    int q2_len = 0;

    queue_time time = 0;

    while (q1_elems < CYCLE_LEN && q1_elems < ENDLESS_LOOP_GUARD)
    {
        DBG_PRINT("q1 elems %d\n", q1_elems);
        DBG_PRINT("q1 len: %d \tq2 len %d \tcoll len: %d\n", q1->len, q2->len, coll->len);

        add_res = l_add_rnd_elem(q1, q2, time);

        add_coll_res = l_add_to_call(q1, q2, coll);

        pop_coll_res = l_pop_from_coll(coll, res);


        q1_elems += pop_coll_res == 123 ? 0 : pop_coll_res;

        if ((q1_elems % EVERY_LOOP_PRINT) == 0 && q1_elems != last_q1)
        {
            last_q1 = q1_elems;

            q1_len += q1->len;
            q2_len += q2->len;

            printf("\n\nТекущая длина T1: %d\n", q1->len);
            printf("Текущая длина T2: %d\n", q2->len);


            printf("Средняя длина T1: %d\n", q1_len / (q1_elems / EVERY_LOOP_PRINT));
            printf("Средняя длина T2: %d\n", q2_len / (q1_elems / EVERY_LOOP_PRINT));

            printf("Кол-во вошедших заявок: %d\n", q1->was + q2->was);
            printf("Кол-во вышедших заявок: %d\n", res->coll);

            printf("Среднее время пребывания в очереди: %lf\n", res->service_time / (res->coll));
            printf("Q1 elems: %d\n", q1_elems);
        }

        if (pop_coll_res == 123)
        {
            res->waiting_time += TIME_PER_LOOP;
        }

        time += TIME_PER_LOOP;
    }

    res->total_in = q1->was + q2->was;

    clean_lqueues(3, &q1, &q2, &coll);

    INFO_PRINT("Q1 elems %d\n", q1_elems);

    return NO_ERROR;
}

int l_add_rnd_elem(lqueue_t *q1, lqueue_t *q2, queue_time time)
{
    static data_t *queue_elem_1 = NULL;
    static data_t *queue_elem_2 = NULL;
    static queue_time last_time_1 = 0;
    static queue_time last_time_2 = 0;
    static int generated_T1 = 0;
    static int generated_T2 = 0;

    int flip_res = COIN_FLIP();

    if (generated_T1 == 0)
    {
        generated_T1 = 1;
        queue_elem_1 = calloc(1, sizeof(data_t));
        L_INIT_NODE(queue_elem_1, T1);
    }
    else if (generated_T2 == 0)
    {
        generated_T2 = 1;
        queue_elem_2 = calloc(1, sizeof(data_t));
        L_INIT_NODE(queue_elem_2, T2);
    }

    if (generated_T1 == 1 && fabs(last_time_1 - time) >= queue_elem_1->entrance)
    {
        last_time_1 = time;
        q1->was++;

        generated_T1 = 0;

        add_lqueue(q1, queue_elem_1);
    }

    if (generated_T2 == 1 && fabs(last_time_2 - time) >= queue_elem_2->entrance)
    {
        last_time_2 = time;
        q1->was++;

        generated_T2 = 0;

        add_lqueue(q2, queue_elem_2);
    }

    return 0;
}

int l_add_to_call(lqueue_t *q1, lqueue_t *q2, lqueue_t *coll)
{
    int flip_res = T2;

    data_t *dat = NULL;

    if (flip_res == T2)
    {
        int type = T1;
        if (q1->len == 0 && find_lqueue(coll, &type, type_cmp) == NULL)
        {
            INFO_PRINT("T2 exit\n");
            dat = pop_lqueue(q2);
            if (dat != NULL)
            {
                coll->was++;
                return add_lqueue(coll, dat);
            }
        }
    }

    if (flip_res == T2)
    {
        INFO_PRINT("T1 exit\n");
        dat = pop_lqueue(q1);
        
        if (dat != NULL)
        {
            coll->was++;
            return add_lqueue(coll, dat);
        }
    }
    
    return 123;
}

int l_pop_from_coll(lqueue_t *coll, tm_res *res)
{
    res->oa_calls++;
    if (coll->head == NULL)
    {
        return 123;
    }

    // l_increase_time(coll, COLLECTIVE_QUEUE);
    ((data_t*) coll->head->data)->last_service += TIME_PER_LOOP;

    if (coll->head != NULL && ((data_t*) coll->head->data)->last_service >= ((data_t*) coll->head->data)->service)
    {
        data_t *data = pop_lqueue(coll);
        
        INFO_PRINT("Serviced!\n");
        INFO_PRINT("T%d Wait time: %lf\tService time: %lf\n", data->type + 1, data->entrance, data->service);

        res->t1_entrance_time += data->type == T1 ? data->entrance : 0;
        res->t2_entrance_time += data->type == T2 ? data->entrance : 0;
        res->t1_service_time += data->type == T1 ? data->service : 0;
        res->t2_service_time += data->type == T2 ? data->service : 0;
        res->service_time += data->service;

        int type = data->type == T1;

        res->coll++;
        res->t1 += data->type == T1;
        res->t2 += data->type == T2;

        free(data);

        return type;
    }

    return coll->head == NULL ? 123 : 0;
}


