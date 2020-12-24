#include <string.h>
#include <ctype.h>
#include "./include/myio.h"
#include "./include/error.h"
#include "./include/bigfloat.h"

static int preprocess_str(char *str, int *mant, int *exp_len, sign_t *mant_sign, int *exp)
{
    if (str[0] != '+' && str[0] != '-')
        return INCORRECT_SIGN;

    int len = strlen(str);

    *mant_sign = str[0] == '+' ? POSITIVE : NEGATIVE;

    int e = 0;

    *mant = 0;
    *exp_len = 0;

    int before_e = 1;

    for (int i = 1; i < len; i++)
    {
        if (tolower(str[i]) == 'e')
        {
            e++;
            before_e = 0;

            if (str[i + 1] != '+' && str[i + 1] != '-')
            {
                return SIGN_AFTER_E;
            }

            *exp *= str[++i] == '+' ? POSITIVE : -1;
        }
        else if (str[i] == '.')
        {
            if (before_e == 0)
                return INCORRECT_DIGIT;
        }
        else if (isdigit(str[i]))
        {
            if (before_e)
                (*mant)++;
            else
                (*exp_len)++;
        }
        else
            return INCORRECT_DIGIT;
    }

    if (e != 1)
        return INCORRECT_E;

    if (*mant > MANTISSA_LENGTH - 1)
        return TOO_LONG_MANT;

    if (*exp_len > EXP_LENGTH)
        return TOO_LONG_EXP;
    
    if (*mant == 0)
        return ZERO_LEN_MANT;
    
    if (*exp_len == 0)
        return ZERO_LEN_EXP;

    return NO_ERROR;
}


void hello_print(void)
{
    puts(
        "Вычисление частного двух действительный чисел\n"
        "Правила ввода действительного числа:\n"
        "\t1. Знак перед числом обязателен к вводу: + или -\n"
        "\t2. Знак экспоненты обязателен к вводу: 'e'или 'E'\n"
        "\t3. Знак порядка обязателен к вводу: + или -\n"
        "\t4. Можно написать ведущие нули\n"
        "\t5. Целая часть отделяется от дробной исключительно точкой: '.'\n"
        "\t6. Ограничения на ввод действительного числа: \n\t\tмаксимальная длина - 39 символов: 1 символ на знак числа,"
        "1 на точку, \n\t\t30 символов на мантиссу, \n\t\t1 символ на знак порядка, \n\t\t1 символ на знак экспоненты, \n\t\t5 цифр на порядок.\n"
        "Пример ввода: +123.3e+3\n"
        "Правила ввода целого числа:\n"
        "\t1. Максимальная длинна целой части - 30 цифр\n"
        "\t2. Первый знак всегда '+' или '-' и он обязателен к вводу\n"
        "Пример вводы: +123412\n"
        );
}

my_error_t input_bigfloat(bigfloat_t *src)
{
    puts("Введите действительное число – пример: -341.3e+4");
    char buff[BUFF_SIZE];
    
    scanf("%s", buff);
    
    return parse_float(buff, src);
}


my_error_t parse_float(char *str, bigfloat_t *dst)
{
    memset(dst->mantissa, 0, MANTISSA_LENGTH);
    dst->exponent = 1;

    int mant;
    int exp;

    int status = preprocess_str(str, &mant, &exp, &dst->mant_sign, &dst->exponent);

    if (status)
        return status;

    status = find_dot_ind(str, dst);

    if (status)
        return status;

    str_to_int(str, &dst->exponent, exp);
    
    int k = 0;
    int i = strlen(str) - 1;

    for (int j = 1; j <= mant + (dst->dot_pos != MANTISSA_LENGTH); j++)
    {
        if (str[j] != '.')
        {
            dst->mantissa[k++] = str[j] - '0';
        }
        i--;
    }

    dst->mant_len = mant;

    return NO_ERROR;
}

my_error_t parse_long_int(const char *src, bigfloat_t *dst)
{
    if (src[0] != '+' && src[0] != '-')
        return INCORRECT_SIGN;

    dst->mant_sign = src[0] == '+' ? POSITIVE : NEGATIVE;

    int len = strlen(src + 1);

    if (!len)
        return ZERO_LEN_BIGINT;

    if (len > MANTISSA_LENGTH - 1)
        return TOO_LONG_BIGINT;

    for (int i = 1; i < len + 1; i++)
    {
        if (!isdigit(src[i]))
        {
            return INCORRECT_DIGIT;
        }
        
        dst->mantissa[dst->mant_len++] = src[i] - '0';
    }

    dst->dot_pos = MANTISSA_LENGTH;

    return NO_ERROR;
}


my_error_t input_long_int(bigfloat_t *src)
{
    puts("Введите целое число – пример: +341");
    char buff[BUFF_SIZE];
    
    scanf("%s", buff);
    
    return parse_long_int(buff, src);
}

my_error_t find_dot_ind(char *str, bigfloat_t *dst)
{
    int len = strlen(str);
    int dot_ind = 0;
    int e_found = 0;
    int dot_found = 0;

    for (int i = 0; i < len; i++)
    {
        if (str[i] == '.')
            dot_found++;

        else if (tolower(str[i]) == 'e')
            e_found = 1;
        else if (!e_found && !dot_found)
            dot_ind++;

    }
    if (dot_found > 1)
        return SINGLETON_DOT;
    
    if (dot_found && !e_found)
        return INCORRECT_E;

    if (dot_found)
        dst->dot_pos = dot_ind - 1;
    else
        dst->dot_pos = MANTISSA_LENGTH;
    
    return NO_ERROR;
}

void print_res(bigfloat_t res)
{
    printf("Результат: %c0.", res.mant_sign ? '+' : '-');
    for (int i = 1; i < res.mant_len; i++)
        printf("%d", res.mantissa[i]);
    printf("E%s%d\n", res.exponent > 0 ? "+" : "", res.exponent);
}


