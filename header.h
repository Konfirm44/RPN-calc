#ifndef HEADER_H
#define HEADER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define num_len_max 24
#define exp_len_max 1024
#define n_operations 5
#define restricted_characters "0123456789.+-*/^"
#define error_msg \
	"You should not be seeing this message. \
If you do, a critical error has occured. Please contact the app developer.\n"

#define argv(x, y) argv[x][y]

extern unsigned int asrt_count;
#define asrt(z)                                                      \
	if (!z) {                                                        \
		++asrt_count;                                                \
		fprintf(stderr, error_msg);                                  \
		fprintf(stderr, "error@ %s, line %d\n", __FILE__, __LINE__); \
		return 0;                                                    \
	}
//

typedef struct stack {
	double value;
	struct stack* next;
} stack;

typedef struct handle {
	stack* head;
	double memory;
	unsigned int stacksize;
} handle;

typedef struct args {
	int should_exit;
	char* infile;
	char* outfile;
	char* xmlfile;
	char whitespace;
	char comment;
	char quit;
	char precision;
	char deleter;
	char memory;
} args;

typedef struct operation {
	double (*fn_ptr)(const double operands[]);
	const char* tag;
	unsigned int num_of_operands;
} operation;

// operations
#define op_function_declr(name) op_##name(const double operands[])

double op_function_declr(add);

double op_function_declr(subtract);

double op_function_declr(multiply);

double op_function_declr(divide);

double op_function_declr(pow);

const operation* get_operation(const char* str);

double* get_operands(handle* const top, unsigned int num_of_operands);

// stack
bool push(handle* const top, const double d);

bool pop(handle* const top, double* d);

double peek(handle* const top);

void pulverize(handle* const top);

void print_stack(handle* const top);  // do piachu

// misc
void get_help();

bool copy_string(char** destination, const char* source);

args parse_args(int argc, char** argv);

handle* new_stack();

bool is_number(const char* ptr, double* d);

bool memory_operation(handle* const top, const char* op);

bool parse_exp(char* exp, handle* const top, const args config, FILE* f_out);

bool read_text(const args config);

#endif  // HEADER_H
