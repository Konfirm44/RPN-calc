/** @file */
#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "stack.h"

#define N_OPERATIONS 5  /// rozmiar tablicy zawierającej wskaźniki na funkcje

/** element tablicy wskaźników na funkcje */
typedef struct operation {
	double (*fn_ptr)(const double operands[]);
	const char* tag;
	unsigned int num_of_operands;
} operation;

/** makro deklarujące następujące funkcje 
 * @param name nazwa funkcji
 * */
#define op_function_declr(name) op_##name(const double operands[])

/** funkcja realizująca dodawanie */
double op_function_declr(add);

/** funkcja realizująca odejmowanie */
double op_function_declr(subtract);

/** funkcja realizująca mnożenie */
double op_function_declr(multiply);

/** funkcja realizująca dzielenie */
double op_function_declr(divide);

/** funkcja realizująca potęgowanie */
double op_function_declr(pow);

/** funkcja realizuje operację na pamięci kalkulatora w oparciu o podany operator pamięciowy
 * @param top uchwyt stosu
 * @param op string zawierający operator pamięciowy
 * @return true = powodzenie
 * */
bool memory_operation(handle* const top, const char* op);

/** funkcja zwraca wskaźnik na funkcję realizującą odpowiednią operację
 * @param str operator
 * @return wskaźnik na funkcję lub NULL, jeżeli funkcja o podanym operatorze nie istnieje
 * */
const operation* get_operation(const char* str);

/** funkcja zwraca tablicę operandów
 * @param top uchwyt stosu
 * @param num_of_operands żądana liczba operandów (a także rozmiar zwracanej tablicy)
 * @return wskaźnik na pierwszy element tablicy
 * */
double* get_operands(handle* const top, unsigned int num_of_operands);

#endif  // OPERATIONS_H