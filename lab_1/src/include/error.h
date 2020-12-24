#ifndef __ERROR_H__
#define __ERROR_H__

#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define RED_COLOR               "\033[0;31m"
#define RESET_COLOR             "\033[0m"

#define RED_MSG(msg) (RED_COLOR msg RESET_COLOR)
#define GREETINGS_PRINT puts(GREETINGS_MSG());

typedef enum my_error_e
{
    NO_ERROR,
    DIVISION_BY_ZERO,
    OVERFLOW_MANT,
    OVERFLOW_EXP,
    INCORRECT_E,
    INCORRECT_SIGN,
    INCORRECT_DIGIT,
    SINGLETON_DOT,
    TOO_LONG_MANT,
    TOO_LONG_EXP,
    SIGN_AFTER_E,
    ZERO_LEN_MANT,
    ZERO_LEN_EXP,
    TOO_LONG_BIGINT,
    ZERO_LEN_BIGINT
} my_error_t;

#define DIVISION_BY_ZERO_MSG() (RED_MSG("Ошибка: деление на ноль"))
#define OVERFLOW_MANT_MSG() (RED_MSG("Ошибка: переполнение мантиссы"))
#define OVERFLOW_EXP_MSG() (RED_MSG("Ошибка: переполнение порядка"))
#define INCORRECT_E_MSG() (RED_MSG("Ошибка: в числе должен быть только один символ E"))
#define INCORRECT_SIGN_MSG() (RED_MSG("Ошибка: первый знак в числе должен быть '+' или '-'"))
#define INCORRECT_DIGIT_MSG() (RED_MSG("Ошибка: был встречен некорректный символ"))
#define SINGLETON_DOT_MSG() (RED_MSG("Ошибка: точка должна быть только одна"))
#define TOO_LONG_MANT_MSG() (RED_MSG("Ошибка: была введена слишком длинная мантисса. Предел – 30 символов"))
#define TOO_LONG_EXP_MSG() (RED_MSG("Ошибка: был введен слишком длинный порядок. Предел - 5 символов"))
#define SIGN_AFTER_E_MSG() (RED_MSG("Ошибка: после знака E должен стоять знак '+' или '-'"))
#define ZERO_LEN_MANT_MSG() (RED_MSG("Ошибка: Вы не ввели мантиссу числа"))
#define ZERO_LEN_EXP_MSG() (RED_MSG("Ошибка: Вы не ввели порядок числа"))
#define TOO_LONG_BIGINT_MSG() (RED_MSG("Ошибка: Вы ввели слишком длинное целое число. Максимальная длинна 30 символов"))
#define ZERO_LEN_BIGINT_MSG() (RED_MSG("Ошибка: Вы не ввели длинное целое число."))
// #define INCORRECT_INPUT_MSG() (RED_MSG("Ошибка: неправильный ввод. Обратите внимание на пример."))

char *mystrerr(my_error_t err);

#endif // __ERROR_H__