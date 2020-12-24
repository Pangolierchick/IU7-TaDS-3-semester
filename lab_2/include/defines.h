#ifndef __DEFINES_H__
#define __DEFINES_H__

#define BRAND_LEN                   20
#define COUNTRY_LEN                 20
#define COLOR_LEN                   10
#define CONDITION_LEN               20

typedef enum new_e
{
    NO,
    YES
} new_t;

typedef struct old_auto_s
{
    int prod_year;
    int mileage;
    int repair_num;
    int owner_num;
} old_auto_t;

typedef struct new_auto_s
{
    int warranty;
} new_auto_t;

typedef union condition_u
{
    new_auto_t new_auto;
    old_auto_t old_auto;
} condition_t;

typedef struct auto_s
{
    char brand[BRAND_LEN + 1];
    char country[COUNTRY_LEN + 1];
    int cost;
    char color[COLOR_LEN + 1];
    new_t is_new;
    condition_t condition;
} auto_t;

typedef auto_t vector_type;

#endif // __DEFINES_H__