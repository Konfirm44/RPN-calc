/** @file */
#ifndef MISC_H
#define MISC_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"
#include "stack.h"

#define exp_len_max 1024 /// najdłuższa możliwa liczba znaków w pojedynczym wyrażeniu
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

#endif // MISC_H