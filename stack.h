/** @file */
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

/** element stosu */
typedef struct stack {
	double value;
	struct stack* next;
} stack;

/** uchwyt stosu */
typedef struct handle {
	stack* head;
	double memory;
	unsigned int stacksize;
} handle;

/** funkcja wstawia liczbę na szczyt stosu
 * @param top uchwyt stosu
 * @param d liczba wstawiana na stos
 * @return true = powodzenie
 * */
bool push(handle* const top, const double d);

/** funkcja zdejmuje liczbę ze szczytu stosu
 * @param top uchwyt stosu
 * @param d wskaźnik na zmienną, do której zwracana jest liczba zdjęta ze stosu
 * @return true = powodzenie
 * */
bool pop(handle* const top, double* d);

/** funkcja zwraca wartość liczby ze szczytu stosu bez zdejmowania jej
 * @param top uchwyt stosu
 * @return wartość liczby ze szczytu stosu, jeżeli stos jest pusty, to funkcja zwróci 0
 * */
double peek(handle* const top);

/** funkcja niszczy stos
 * @param top uchwyt stosu
 * */
void pulverize(handle* const top);

#endif  // STACK_H