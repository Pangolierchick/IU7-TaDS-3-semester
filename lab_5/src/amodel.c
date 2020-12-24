#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "logger.h"
#include "amodel.h"
#include "aqueue.h"
#include "errors.h"

#define ENTRANCE_TIME(aqueue) (aqueue->arr[0].last_entrance)
#define SERVICE_TIME(aqueue)  (aqueue->arr[0].last_service)

#define NEEDED_ENTRANCE_TIME(aqueue) (aqueue->arr[0].entrance)
#define NEEDED_SERVICE_TIME(aqueue)  (aqueue->arr[0].service)

void a_increase_time(aqueue_t *q, int type)
{
    if (type == INDIVIDUAL_QUEUE)
        q->arr[0].last_entrance += TIME_PER_LOOP;
    else
        q->arr[0].last_service += TIME_PER_LOOP;
}

static int type_cmp(void *a, void *b)
{
    data_t *pa = (data_t *) a;
    queue_type_t pb = *((queue_type_t *) b);

    return pb == pa->type;
}

#define INIT_NODE(__node, __type)                                                       \
    do                                                                                  \
    {                                                                                   \
        __node.entrance = lfrnd(__type##_WAITING_TIME_MIN, __type##_WAITING_TIME_MAX);  \
        __node.service = lfrnd(__type##_SERVICE_TIME_MIN, __type##_SERVICE_TIME_MAX);   \
        __node.last_entrance = 0;                                                       \
        __node.last_service = 0;                                                        \
        __node.type = __type;                                                           \
    } while (0);

int a_queue_model(tm_res *res)
{
    aqueue_t *q1 = init_aqueue();
    aqueue_t *q2 = init_aqueue();
    aqueue_t *coll = init_aqueue();

    if (!(q1 && q2 && coll))
    {
        clean_aqueues(3, &q1, &q2, &coll);

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

        add_res = a_add_rnd_elem(q1, q2, time);

        add_coll_res = a_add_to_call(q1, q2, coll);

        pop_coll_res = a_pop_from_coll(coll, res);

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

    clean_aqueues(3, &q1, &q2, &coll);

    INFO_PRINT("Q1 elems %d\n", q1_elems);


    printf("avg: %lf\n", res->avg_wait);

    return NO_ERROR;
}

int a_add_rnd_elem(aqueue_t *q1, aqueue_t *q2, queue_time time)
{
    static data_t queue_elem_1;
    static data_t queue_elem_2;
    static queue_time last_time_1 = 0;
    static queue_time last_time_2 = 0;
    static int generated_T1 = 0;
    static int generated_T2 = 0;

    if (generated_T1 == 0)
    {
        generated_T1 = 1;
        INIT_NODE(queue_elem_1, T1);
    }
    
    if (generated_T2 == 0)
    {
        generated_T2 = 1;
        INIT_NODE(queue_elem_2, T2);
    }

    if (fabs(last_time_1 - time) >= queue_elem_1.entrance)
    {
        last_time_1 = time;

        q1->was++;
        generated_T1 = 0;
        add_aqueue(q1, queue_elem_1);
    }

    if (fabs(last_time_2 - time) >= queue_elem_2.entrance)
    {
        last_time_2 = time;

        q2->was++;
        generated_T2 = 0;
        add_aqueue(q2, queue_elem_2);
    }
    
    return 0;
}

int a_add_to_call(aqueue_t *q1, aqueue_t *q2, aqueue_t *coll)
{
    int flip_res = T2;

    data_t dat;

    if (flip_res == T2)
    {
        queue_type_t type = T1;
        if (q1->len == 0 && find_aqueue(coll, &type, type_cmp) == NULL)
        {
            INFO_PRINT("T2 exit\n");
            if (!pop_aqueue(q2, &dat))
            {
                coll->was++;
                return add_aqueue(coll, dat);
            }
        }
    }

    if (flip_res == T2)
    {
        INFO_PRINT("T1 exit\n");
        if (!pop_aqueue(q1, &dat))
        {
            coll->was++;
            return add_aqueue(coll, dat);
        }
    }

    return 123;
}

int a_pop_from_coll(aqueue_t *coll, tm_res *res)
{
    res->oa_calls++;
    if (coll->len == 0)
        return 123;

    coll->arr[0].last_service += TIME_PER_LOOP;

    if (coll->len && coll->arr[0].last_service >= coll->arr[0].service)
    {
        data_t data;
        pop_aqueue(coll, &data);
        
        INFO_PRINT("Serviced!\n");
        INFO_PRINT("T%d Wait time: %lf\tService time: %lf\n", data.type + 1, data.entrance, data.service);

        res->t1_entrance_time += data.type == T1 ? data.entrance : 0;
        res->t2_entrance_time += data.type == T2 ? data.entrance : 0;
        res->t1_service_time += data.type == T1 ? data.service : 0;
        res->t2_service_time += data.type == T2 ? data.service : 0;
        res->service_time += data.service;

        res->avg_wait += data.entrance;

        int type = data.type == T1;

        res->coll++;
        res->t1 += data.type == T1;
        res->t2 += data.type == T2;

        return type;
    }

    return coll->len == 0 ? 123 : 0;
}


