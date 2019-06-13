/** @file */
#ifndef HEADER_H
#define HEADER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define exp_len_max 1024 /// najdłuższa możliwa liczba znaków w pojedynczym wyrażeniu
#define n_operations 5 /// rozmiar tablicy zawierającej wskaźniki na funkcje
#define restricted_characters "0123456789.+-*/^" /// znaki, których nie można używać jako inicjatorów komentarza itd. 
#define error_msg \
	"You should not be seeing this message. \
If you do, a critical error has occured. Please contact the app developer.\n"

#define argv(x, y) argv[x][y] /// wygodniejszy dostęp do tablicy dwuwymiarowej
#define eq(str1, str2) !strcmp(str1, str2) /// wygodniejsze porównywanie stringów

extern unsigned int asrt_count; /// inicjalizacja w main.c

/** makro wychwytujące błędy krytyczne */
#define asrt(z)                                                      \
	if (!z) {                                                        \
		++asrt_count;                                                \
		fprintf(stderr, error_msg);                                  \
		fprintf(stderr, "error@ %s, line %d\n", __FILE__, __LINE__); \
		return 0;                                                    \
	}
//

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

/** argumenty wiersza poleceń */
typedef struct args {
	int should_exit; ///< informacja o błędnych argumentach
	char* infile; ///< ścieżka do pliku wejściowego
	char* outfile; ///< ścieżka do pliku wyjściowego
	char whitespace; ///< znak odstępu między kolejnymi operatorami/operandami
	char comment; ///< znak/operator rozpoczęcia komentarza
	char quit; ///< znak/operator zamknięcia programu
	char precision; ///< długość rozwinięcia dziesiętnego dla wyjścia programu
	char deleter; ///< znak/operator czyszczenia stosu
	char memory; ///< znak/operator rozpoczynający operatory pamięciowe
} args;

/** element tablicy wskaźników na funkcje */
typedef struct operation {
	double (*fn_ptr)(const double operands[]);
	const char* tag;
	unsigned int num_of_operands;
} operation;






// operations ================================================================

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

// stack ================================================================

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
 * @return wartość liczby ze szczytu stosu
 * */
double peek(handle* const top);

/** funkcja niszczy stos
 * @param top uchwyt stosu
 * */
void pulverize(handle* const top);

// misc ========================================================================

/** funkcja wyświetla pomoc */
void get_help();

/** funkcja sprawdza poprawność podanej ścieżki dostępu do pliku, po czym alokuje pamięć i kopiuje ścieżkę
 * @param destination wskaźnik na pamięć, która będzie alokowana
 * @param source ścieżka dostępu do pliku
 * @return true = powodzenie
 * */
bool copy_path(char** destination, const char* source);

/** funkcja przetwarza argumenty wiersza poleceń
 * @return struktura args przechowująca informacje istotne dla programu
 * */
args parse_args(int argc, char** argv);

/** funkcja alokuje i inicjalizuje nowy uchwyt stosu 
 * @return uchwyt stosu lub NULL w przypadku niepowodzenia alokacji
 * */
handle* new_stack();

/** funkcja sprawdza, czy podany string zawiera poprawną liczbę i zwraca ją
 * @param ptr string do sprawdzenia
 * @param d wskaźnik na zmienną, do której zostanie zwrócona liczba
 * @return true = string zawiera liczbę
 * */
bool is_number(const char* ptr, double* d);

/** funkcja realizuje operację na pamięci kalkulatora w oparciu o podany operator pamięciowy
 * @param top uchwyt stosu
 * @param op string zawierający operator pamięciowy
 * @return true = powodzenie
 * */
bool memory_operation(handle* const top, const char* op);

/** funkcja przetwarza wyrażenie w ONP podane w stringu
 * @param exp przetwarzany string
 * @param top uchwyt stosu
 * @param config konfiguracja programu
 * @param f_out strumień wyjścia programu
 * @return true = powodzenie
 * */
bool parse_exp(char* exp, handle* const top, const args config, FILE* f_out);

/** funkcja 
 * @param config konfiguracja programu
 * @return true = powodzenie
 * */
bool read_text(const args config);

#endif  // HEADER_H
