#include <stdio.h>
#include <stdlib.h>
#include "io.h"

void print_hello(void)
{
    system("figlet -f slant 'Sparse matrixes'");
    puts("Данная программа производит умножение вектора-строки на матрицу в обычном виде или разреженном.");
}

void print_help(void)
{
    puts("\n\n1  -- ввод матрицы.");
    puts("2  -- генерация случайной матрицы.");
    puts("3  -- ввод вектора-строки.");
    puts("4  -- генерация случайного вектора-строки");
    puts("5  -- умножение обычным алгоритмом.");
    puts("6  -- умножение в разреженном виде.");
    puts("7  -- вывод результата умножения.");
    puts("8  -- очистить экран.");
    puts("9  -- напечатать матрицу на экран");
    puts("10 -- напечатать вектор на экран");
    puts("0  -- выход из программы.");
}

int ask_y_n(const char *msg)
{
    printf("%s (y/n): ", msg); 
    
    char answer;
    
    fpurge(stdin);
    scanf("%c", &answer);

    return answer == 'y';
}