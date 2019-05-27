#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdbool.h>
#include <stdio.h>

extern unsigned int err_count;

#define num_len_max 24
#define exp_len_max 1024
#define n_operations 5
#define restricted_characters "0123456789.+-*/^xXmMrRcC"
#define error_msg \
	"You should not be seeing this message. \
If you do, a critical error has occured. Please contact the app developer.\n"

#define argv(x, y) argv[x][y]
#define debug fprintf(stderr, "error@ %s, line %d\n", __FILE__, __LINE__)
#define asrt(z)                     \
	if (!z) {                       \
		++err_count;				\
		fprintf(stderr, error_msg); \
		debug;                      \
		return 0;                   \
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
	bool should_exit;
	char* infile;
	char* outfile;
	char* xmlfile;
	char whitespace;
	char comment;
	char abandon;
	char precision;
} args;

typedef struct operation{
	double (*fn_ptr)(const double operands[]);
	const char* tag;
	unsigned int num_of_operands;
} operation;

#define op_function_declr(name, op) op_##name(const double operands[])

double op_function_declr(add, +);

double op_function_declr(subtract, -);

double op_function_declr(multiply, *);

double op_function_declr(divide, /);

double op_function_declr(pow, ^);

bool push(handle* const top, const double d);

bool pop(handle* const top, double* d);

double peek(handle* const top);

void pulverize(handle* const top);

void print_stack(handle* const top);

int solve(handle* const top, const char op);

int is_op(const char c);		

int memory_operation(handle* const top, const char op);

handle* new_stack();

bool is_number(const char* ptr, double* d);

int parse_exp(char* exp, handle* const top, const args xd, FILE* f_out);
//returny:
//-1 komentarz
//0 = error@stack
//1 alles gut

int copy_string(char** destination, const char* source);

args parse_args(int argc, char** argv);

void get_help();

#endif  // HEADERS_H_
