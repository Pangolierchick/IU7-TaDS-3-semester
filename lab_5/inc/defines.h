#ifndef __DEFINES_H__
#define __DEFINES_H__

extern int print_pointers;

#define CYCLE_LEN                   1000
#define ENDLESS_LOOP_GUARD          10000

#define EVERY_LOOP_PRINT            100

#define TIME_PER_LOOP              1

#define __GET_RND(__x)                 ((rand() % 100) > __x)
#define COIN_FLIP()                    __GET_RND(50)


#define INDIVIDUAL_QUEUE            0
#define COLLECTIVE_QUEUE            1


// ------------------------------

#define T1_WAITING_TIME_MIN         1
#define T1_WAITING_TIME_MAX         5

#define T2_WAITING_TIME_MIN         0
#define T2_WAITING_TIME_MAX         3

// ------------------------------

#define T1_SERVICE_TIME_MIN         0
#define T1_SERVICE_TIME_MAX         4

#define T2_SERVICE_TIME_MIN         0
#define T2_SERVICE_TIME_MAX         1

#define T1_WAITING_MIDDLE       ((T1_WAITING_TIME_MAX + T1_WAITING_TIME_MIN) / 2.0)    
#define T2_WAITING_MIDDLE       ((T2_WAITING_TIME_MAX + T2_WAITING_TIME_MIN) / 2.0)
#define T1_SERVICE_MIDDLE       ((T1_SERVICE_TIME_MAX + T1_SERVICE_TIME_MIN) / 2.0)    
#define T2_SERVICE_MIDDLE       ((T2_SERVICE_TIME_MAX + T2_SERVICE_TIME_MIN) / 2.0)

#define WAITING_TIME_MIDDLE         (((T1_WAITING_TIME_MAX + T1_WAITING_TIME_MIN) / 2.0) + ((T2_WAITING_TIME_MAX + T2_WAITING_TIME_MIN) / 2.0)) / 2.0
#define SERVICE_TIME_MIDDLE         (((T1_SERVICE_TIME_MAX + T1_SERVICE_TIME_MIN) / 2.0) + ((T2_SERVICE_TIME_MAX + T2_SERVICE_TIME_MIN) / 2.0)) / 2.0

// ------------------------------

typedef enum queue_type_e
{
    T1,
    T2
} queue_type_t;

typedef double queue_time;

typedef struct data_s
{
    queue_time   last_entrance;
    queue_time   last_service;

    queue_time   entrance;
    queue_time   service;

    queue_type_t type;
} data_t;

typedef struct tm_res_s
{
    double t1_entrance_time;
    double t2_entrance_time;
    double t1_service_time;
    double t2_service_time;
    double waiting_time;
    double service_time;

    int t1;
    int t2;
    int coll;

    int total_in;

    double avg_wait;

    int oa_calls;
} tm_res;

#define lfrnd(min, max) (min + ((double) rand() / RAND_MAX * (max - min)))

#define ask_y_n(msg, v)         \
    do                          \
    {                           \
        char c;                 \
        printf(msg);            \
        fpurge(stdin);          \
        scanf("%c", &c);        \
        v = c == 'y';           \
    } while (0);                \
    

#endif // __DEFINES_H__
