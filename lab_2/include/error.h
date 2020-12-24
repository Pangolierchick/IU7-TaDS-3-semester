#ifndef __ERROR_H__
#define __ERROR_H__

#ifndef NO_ERROR
#define NO_ERROR                0
#endif

#define BAD_BRAND               1
#define BAD_COUNTRY             2
#define BAD_COST                3
#define BAD_COLOR               4
#define BAD_YES_NO              5
#define BAD_WARRANTY            6
#define BAD_PROD_YEAR           7
#define BAD_MILEAGE             8
#define BAD_REPAIR_NUM          9
#define BAD_OWNER_NUM          10
#define BAD_RECORD_NUM         11
#define BAD_FILENAME           12
#define UNEXISTING_FILE        13
#define BAD_FILE               14

#define BAD_BRAND_MSG                   "Бренд был введен неверно."
#define BAD_COUNTRY_MSG                 "Страна была введена неверно."
#define BAD_COST_MSG                    "Цена была введена неверно."
#define BAD_COLOR_MSG                   "Цвет машины был введен неверно."
#define BAD_YES_NO_MSG                  "Был неправильно дан ответ на y/n вопрос."
#define BAD_WARRANTY_MSG                "Гарантия была введена неправильно."
#define BAD_PROD_YEAR_MSG               "Год производства был введен неправильно."
#define BAD_MILEAGE_MSG                 "Пробег был введен неправильно."
#define BAD_REPAIR_NUM_MSG              "Количество починок было введено неправильно."
#define BAD_OWNER_NUM_MSG               "Количество бывших владельцов было введено неправильно."
#define BAD_RECORD_NUM_MSG              "Номер записи был введен неправильно."
#define BAD_FILENAME_MSG                "Было введено неправильное имя файла."
#define UNEXISTING_FILE_MSG             "Такой файла не существует."
#define BAD_FILE_MSG                    "Неправильный файл."
#define INDEX_OUT_OF_BOUNDS_MSG         "Неправильный индекс вектора."
#define VECTOR_UNDERFLOW_MSG            "Попытка удалить элемент из пустого вектора."
#define VECTOR_OVERFLOW_MSG             "Произошло переполнение вектора."


char *mystrerr(int errnum);


#endif // __ERROR_H__