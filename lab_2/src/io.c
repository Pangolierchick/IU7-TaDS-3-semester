#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "defines.h"
#include "error.h"
#include "static_vector.h"

int ask_y_n(const char *msg)
{
    printf(msg);

    char answer;

    fpurge(stdin);
    if (scanf("%c", &answer) != 1)
        return BAD_YES_NO;
    
    if (answer != 'y' && answer != 'n')
        return BAD_YES_NO;
    
    return answer == 'y';
}

void print_help(void)
{
    puts("Доступные команды:");
    puts("\n\tprint -- печать таблицы");
    puts("\tadd -- добавление записи в таблицу");
    puts("\tdel -- удаление записи из таблицы");
    puts("\tbsort -- сортировка пузырьком по таблице или по массиву ключей");
    puts("\tqsort -- быстрая сортировка по таблице или по массиву ключей");
    puts("\ttask -- выполнение поиска записей по заданию");
    puts("\tload -- загрузить таблицу из файла");
    puts("\tdump -- сохранить таблицу в файл");
    puts("\tclear -- очистить экран");
    // puts("\thelp -- вывести сообщение о всех командах");
    puts("\texit -- выйти из программы\n");
}

void print_hello(void)
{
    system("figlet -f slant 'SORT CONQUEST'");
    puts("Данная программа выполняет сортировку таблиц автомобилей, выводит цены не новых машин указанной марки\n"\
    "с одним предыдущим собственником , отсутствием ремонта в указанном диапазоне цен.\n");
    print_help();
}

int read_one_car(vector_type *car)
{
    fpurge(stdin);
    printf("Введите марку машины: ");

    if (scanf("%21s", car->brand) != 1)
        return BAD_BRAND;

    if (strlen(car->brand) > BRAND_LEN)
        return BAD_BRAND;
    
    fpurge(stdin);
    printf("Введите страну производства: ");
    if (scanf("%21s", car->country) != 1)
        return BAD_COUNTRY;
    
    if (strlen(car->country) > COUNTRY_LEN)
        return BAD_COUNTRY;
    
    fpurge(stdin);
    printf("Введите стоимость автомобиля: ");
    if (scanf("%d", &car->cost) != 1)
        return BAD_COST;
    
    if (car->cost <= 0)
        return BAD_COST;
    
    fpurge(stdin);
    printf("Введите цвет машины: ");
    if (scanf("%11s", car->color) != 1)
        return BAD_COLOR;
    
    if (strlen(car->color) > COLOR_LEN)
        return BAD_COLOR;

    char yes_no;
    
    fpurge(stdin);
    printf("Эта машина новая? (y/n)");
    if (scanf("%c", &yes_no) != 1)
        return BAD_YES_NO;

    if (yes_no != 'y' && yes_no != 'n')
        return BAD_YES_NO;
    
    car->is_new = yes_no == 'y' ? YES : NO;

    if (yes_no == 'y')
    {
        printf("Введите срок действия гарантии: ");
        if (scanf("%d", &car->condition.new_auto.warranty) != 1)
            return BAD_WARRANTY;
        
        if (car->condition.new_auto.warranty < 0)
            return BAD_WARRANTY;

    }
    else
    {
        printf("Введите год производства: ");
        if (scanf("%d", &car->condition.old_auto.prod_year) != 1)
            return BAD_PROD_YEAR;
        
        if (car->condition.old_auto.prod_year <= 0)
            return BAD_PROD_YEAR;
        
        printf("Введите пробег: ");
        if (scanf("%d", &car->condition.old_auto.mileage) != 1)
            return BAD_MILEAGE;

        if (car->condition.old_auto.mileage < 0)
            return BAD_MILEAGE;
        
        printf("Введите кол-во починок: ");
        if (scanf("%d", &car->condition.old_auto.repair_num) != 1)
            return BAD_REPAIR_NUM;
        
        if (car->condition.old_auto.repair_num < 0)
            return BAD_REPAIR_NUM;
        
        printf("Введите кол-во бывших владельцов: ");
        if (scanf("%d", &car->condition.old_auto.owner_num) != 1)
            return BAD_OWNER_NUM;
        
        if (car->condition.old_auto.owner_num < 1)
            return BAD_OWNER_NUM;
    }

    return NO_ERROR;
}


// FRAGILE!!! Do not touch it.
void print_table(const static_vector_t table)
{
    if (table.len == 0)
    {
        puts("Таблица пуста!");
        return;
    }

    #if DEBUG
    printf("Len: %d\n", table.len);
    #endif

    printf("+-----+--------------------+--------------------+---------------+----------+---------+---------+---------------+---------------+---------------+---------------+\n");
    printf("|  %s  |%20s|%20s|%15s|%10s|%9s|%9s|%15s|%15s|%15s|%15s|\n", "#", "brand", "country", "cost", "color", "is new", "warranty", "prod year", "mileage", "repair num", "owner num");
    printf("+-----+--------------------+--------------------+---------------+----------+---------+---------+---------------+---------------+---------------+---------------+\n");
    for (int i = 0; i < table.len; i++)
    {
        if (table.vec[i].is_new == YES)
        {
            printf("|%5d|%20s|%20s|%15d|%10s|    %c    |%9d|%8c       |%8c       |%8c       |%8c       |\n", i + 1, table.vec[i].brand, 
                                                                                                    table.vec[i].country, 
                                                                                                    table.vec[i].cost,  
                                                                                                    table.vec[i].color,  
                                                                                                    'y',  
                                                                                                    table.vec[i].condition.new_auto.warranty,  
                                                                                                    '-', '-', '-', '-');

        }
        else
        {
            printf("|%5d|%20s|%20s|%15d|%10s|    %c    |    %c    |%15d|%15d|%15d|%15d|\n", i + 1, table.vec[i].brand, 
                                                                                                    table.vec[i].country, 
                                                                                                    table.vec[i].cost,  
                                                                                                    table.vec[i].color,  
                                                                                                    'n',  
                                                                                                    '-',  
                                                                                                    table.vec[i].condition.old_auto.prod_year,
                                                                                                    table.vec[i].condition.old_auto.mileage,
                                                                                                    table.vec[i].condition.old_auto.repair_num,
                                                                                                    table.vec[i].condition.old_auto.owner_num);
        }
        
        printf("+-----+--------------------+--------------------+---------------+----------+---------+---------+---------------+---------------+---------------+---------------+\n");
    }
}

int add_record(static_vector_t *table)
{
    vector_type tmp;
    int res = read_one_car(&tmp);

    if (res)
    {

        return res;
    }
    
    return append(table, tmp);
}

int delete_record(static_vector_t *table)
{   
    if (!table->len)
    {
        puts("Таблица пуста.");
        return NO_ERROR;
    }

    int record_num;

    printf("Введите номер записи, которую вы ходите удалить (1 .. %d): ", table->len);
    
    if (scanf("%d", &record_num) != 1)
        return BAD_RECORD_NUM;
    
    if (record_num < 1 || record_num > table->len + 1)
        return BAD_RECORD_NUM;
    
    vector_type dummy;
    
    return pop(table, record_num - 1, &dummy);
}

int read_car_from_file(FILE *f, vector_type *record)
{
    if (fscanf(f, "%s", record->brand) != 1)
        return FILE_END;

    if (fscanf(f, "%s", record->country) != 1)
        return BAD_FILE;

    if (fscanf(f, "%d", &record->cost) != 1)
        return BAD_FILE;
    
    if (record->cost < 0)
        return BAD_FILE;
    
    if (fscanf(f, "%s", record->color) != 1)
        return BAD_FILE;

    char yes_no;
    
    if (fscanf(f, "%*c%c", &yes_no) != 1)
        return BAD_FILE;

    #if DEBUG
    printf("Yes/No %c\n", yes_no);
    #endif 
    
    if (yes_no != 'y' && yes_no != 'n')
        return BAD_FILE;


    record->is_new = yes_no == 'y' ? YES : NO;

    if (record->is_new)
    {
        if (fscanf(f, "%d", &record->condition.new_auto.warranty) != 1)
            return BAD_FILE;
        
        if (record->condition.new_auto.warranty < 0)
            return BAD_FILE;
        
        #if DEBUG
        printf("warranty: %d\n", record->condition.new_auto.warranty);
        #endif

        return NO_ERROR;
    }
    

    if (fscanf(f, "%d", &record->condition.old_auto.prod_year) != 1)
        return BAD_FILE;
    
    if (record->condition.old_auto.prod_year < 0)
            return BAD_FILE;

    if (fscanf(f, "%d", &record->condition.old_auto.mileage) != 1)
        return BAD_FILE;

    if (record->condition.old_auto.mileage < 0)
        return BAD_FILE;
    
    if (fscanf(f, "%d", &record->condition.old_auto.repair_num) != 1)
        return BAD_FILE;
    
    if (record->condition.old_auto.repair_num < 0)
            return BAD_FILE;
    
    if (fscanf(f, "%d", &record->condition.old_auto.owner_num) != 1)
        return BAD_FILE;
    
    if (record->condition.old_auto.owner_num < 0)
        return BAD_FILE;
    
    return NO_ERROR;
}

int read_from_file(static_vector_t *table)
{
    init_vector(table);

    const int filename_len = 20;
    char filename[filename_len];

    printf("Введите имя файла: ");
    
    fpurge(stdin);

    if (scanf("%20s", filename) != 1)
        return BAD_FILENAME;
    
    FILE *f = fopen(filename, "r");

    if (f == NULL)
        return UNEXISTING_FILE;
    
    #if DEBUG
    int i = 0;
    #endif

    int read_res = NO_ERROR;

    while (read_res == NO_ERROR)
    {
        
        vector_type tmp;
        read_res = read_car_from_file(f, &tmp);

        #if DEBUG
        printf("Current iter: %d\n", ++i);
        printf("Current res: %d\n", read_res);
        #endif

        if (read_res == NO_ERROR)
            if (append(table, tmp))
                return vec_errno;
    }

    fclose(f);

    if (read_res == BAD_FILE)
        return BAD_FILE;
    
    puts("Готово.");
    
    return NO_ERROR;
}

int write_rec_in_file(FILE *f, vector_type *record)
{
    fprintf(f, "%s\n", record->brand);
    fprintf(f, "%s\n", record->country);
    fprintf(f, "%d\n", record->cost);
    fprintf(f, "%s\n", record->color);
    fprintf(f, "%c\n", record->is_new == YES ? 'y' : 'n');
    
    if (record->is_new)
    {
        fprintf(f, "%d\n", record->condition.new_auto.warranty);
    }
    else
    {
        fprintf(f, "%d\n", record->condition.old_auto.prod_year);
        fprintf(f, "%d\n", record->condition.old_auto.mileage);
        fprintf(f, "%d\n", record->condition.old_auto.repair_num);
        fprintf(f, "%d\n", record->condition.old_auto.owner_num);
    }

    return NO_ERROR;
}

int write_in_file(static_vector_t *table)
{
    if (!table->len)
    {
        puts("Таблица пуста.");
        return NO_ERROR;
    }

    const int filename_len = 20;
    char filename[filename_len];

    printf("Введите имя файла: ");
    
    fpurge(stdin);
    if (scanf("%20s", filename) != 1)
        return BAD_FILENAME;
    
    FILE *f = fopen(filename, "w");
    
    for (int i = 0; i < table->len; i++)
        write_rec_in_file(f, &table->vec[i]);
    
    puts("Готово.");
    
    return fclose(f) == EOF;
}