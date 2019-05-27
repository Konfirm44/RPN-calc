#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

#define op_function_def(name, op)                      \
	double op_##name(const double a, const double b) { \
		return a op b;                                 \
	}
//

op_function_def(add, +)

op_function_def(subtract, -)

op_function_def(multiply, *)

op_function_def(divide, /)

double op_pow(const double a, const double b) { return pow(a, b); }

bool push(handle* const top, const double d) {  //picuś glancuś
	stack* p = malloc(sizeof(stack));
	asrt(p);
	p->value = d;
	p->next = top->head;
	top->head = p;
	++(top->stacksize);
	return 1;
}

bool pop(handle* const top, double* d) {  //picuś glancuś
	asrt(top->head);
	*d = top->head->value;
	stack* p = top->head->next;
	free(top->head);
	top->head = p;
	--(top->stacksize);
	return 1;
}

double peek(handle* const top) {  //picuś glancuś
	if (!(top->head))
		return 0;
	return top->head->value;
}

void pulverize(handle* const top) {  //picuś glancuś
	double d;
	while (top->head)
		pop(top, &d);
}

void print_stack(handle* const top) {  //do piachu
	stack* p = top->head;
	printf("MEMORY = %0.2f\nSTACK:\t", top->memory);
	while (p) {
		printf("%.2f\t", p->value);
		p = p->next;
	}
	printf("END OF STACK\n");
}

int solve(handle* const top, const char op) {	//do zaorania też
	asrt(top->head);
	double a, b;
	asrt(pop(top, &b));
	if (!(top->head)) {
		push(top, b);
		return 1;
	}
	asrt(pop(top, &a));
	switch (op) {
	case '+':
		a += b;
		break;
	case '-':
		a -= b;
		break;
	case '*':
		a *= b;
		break;
	case '/':
		a /= b;
		break;
	case '^':
		a = pow(a, b);
		break;
	default:
		return 0;
	}
	asrt(push(top, a));
	return 1;
}

int is_op(const char c) {  //do zaorania
	switch (c) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
		return 1;
	default:
		return 0;
	}
}

int memory_operation(handle* const top, const char op) {
	switch (op) {
	case '+':
		top->memory += peek(top);
		return 1;
	case '-':
		top->memory -= peek(top);
		return 1;
	case 'r':
	case 'R':
		push(top, top->memory);
		return 1;
	case 'c':
	case 'C':
		top->memory = 0;
		return 1;
	default:
		//printf("ERROR: why u call me with op=_%c_\n", op);
		return 0;
	}
	//printf("current memory= %0.2f\n", top->memory);
}

handle* new_stack() {  //picuś glancuś
	handle* top = malloc(sizeof(handle));
	top->head = NULL;
	top->memory = 0;
	top->stacksize = 0;
	return top;
}

bool is_number(const char* ptr, double* d) {  //picuś glancuś
	char* endptr;
	double val = strtof(ptr, &endptr);
	if ((*endptr) == '\0') {
		*d = val;
		return 1;
	}
	return 0;
}

int parse_exp(char* exp, handle* const top, const args xd, FILE* f_out) {
	asrt(exp);
	if (xd.infile)
		fprintf(f_out, "exp= %s\n", exp);

	char delim[2] = " ";
	delim[0] = xd.whitespace;
	char* ptr = strtok(exp, delim);
	while (ptr) {
		double d;
		if (ptr[0] == xd.comment) {
			break;
		} else if (is_number(ptr, &d)) {
			asrt(push(top, d));
		} else if (strlen(ptr) == 1 && is_op(ptr[0])) {
			if (top->stacksize > 1) {
				asrt(solve(top, ptr[0]));
			} else {
				fprintf(stderr, "ERROR: too few operands\n");
				return 0;
			}
		} else if (strlen(ptr) == 2 && (ptr[0] == 'm' || ptr[0] == 'M')) {
			asrt(memory_operation(top, ptr[1]));
		} else {
			fprintf(stderr, "ERROR: invalid token '%s'\n", ptr);
			return 0;
		}
		ptr = strtok(NULL, delim);
	}
	if (top->stacksize > 1) {
		fprintf(stderr, "ERROR: not enough operators; %u operator(s) missing\n", (top->stacksize) - 1);
		pulverize(top);
		return 0;
	}
	return 1;
}

int copy_string(char** destination, const char* source) {  //picuś glancuś
	(*destination) = malloc(strlen(source) + 1);
	asrt(*destination);
	strcpy(*destination, source);
	return 1;
}

args parse_args(int argc, char** argv) {
	args xd = {
		.should_exit = false,
		.infile = NULL,
		.outfile = NULL,
		.xmlfile = NULL,
		.whitespace = ' ',
		.comment = '#',
		.abandon = '$',
		.precision = 2};

	int i;
	for (i = 1; i < argc; ++i) {
		if (argv(i, 0) == '-') {
			if (strlen(argv[i]) == 2) {
				char c = argv(i, 1);
				if (c == 'i') {
					if (i == argc - 1 || !copy_string(&(xd.infile), argv[++i]))
						xd.should_exit = true;
				} else if (c == 'o') {
					if (i == argc - 1 || !copy_string(&(xd.outfile), argv[++i]))
						xd.should_exit = true;
				} else if (c == 'x') {
					if (i == argc - 1 || !copy_string(&(xd.xmlfile), argv[++i]))
						xd.should_exit = true;
				} else if (c == 'w') {
					if (i == argc - 1 || strlen(argv[++i]) != 1) {
						xd.should_exit = true;
					} else {
						xd.whitespace = argv(i, 0);
						if (strchr(restricted_characters, xd.whitespace))
							xd.should_exit = true;
					}
				} else if (c == 'c') {
					if (i == argc - 1 || strlen(argv[++i]) != 1) {
						xd.should_exit = true;
					} else {
						xd.comment = argv(i, 0);
						if (strchr(restricted_characters, xd.comment))
							xd.should_exit = true;
					}
				} else if (c == 'a') {
					if (i == argc - 1 || strlen(argv[++i]) != 1) {
						xd.should_exit = true;
					} else {
						xd.abandon = argv(i, 0);
						if (strchr(restricted_characters, xd.abandon))
							xd.should_exit = true;
					}
				} else if (c == 'p') {
					if (i == argc - 1 || strlen(argv[++i]) != 1) {
						xd.should_exit = true;
					} else {
						char* endptr;
						size_t t = strtol(argv[i], &endptr, 10);
						if ((*endptr) != '\0' || t < 0 || t > 15)
							xd.should_exit = true;
						else
							xd.precision = t;
					}
				} else if (c == 'h') {
					get_help();
				} else
					xd.should_exit = true;
			} else
				xd.should_exit = true;
		}
		if (xd.should_exit) {
			fprintf(stderr, "invalid argument #%d:_%s_\nthe program will now exit\n", i + 1, argv[i]);
			break;
		}
	}
	//sprawdz czy nie ma konfliktow # $
	return xd;
}

void get_help() {
	puts("u need momma to help u boi, program will now exit");
	/* 
	i	input
	o	output
	x	xml
	w	whitespace
	c	comment
	a	abandon
	p	precision
	h	help 
	*/
}

/* int parse_exp_old(char* exp, handle* const top, const args xd, FILE* f_out) {
   asrt(exp);
   if (exp[0] == xd.comment) return -1;
   fprintf(f_out, "exp= %s\n", exp);
   char str[num_len_max] = "";
   for (int i = 0; i < strlen(exp); ++i) {
       //printf("%d:_%c_\tlen= %u\n", i, exp[i], strlen(exp));
       if (isdigit(exp[i]) || exp[i] == '.') {
           strncat(str, &(exp[i]), 1);
       }
       else if (exp[i] == xd.whitespace) {
           float f = strtof(str, NULL);
           asrt(push(top, f));
           str[0] = '\0';
       }
       else if (is_op(exp[i])) {
           asrt(solve(top, exp[i]));
           if (exp[i+1] == xd.whitespace)
               ++i;
       }
       else if (exp[i] == 'm' || exp[i] == 'M') {
           asrt(memory_operation(top, exp[++i]));
			if (exp[i+1] == xd.whitespace)
               ++i;
       }
       else if (exp[i] == xd.comment)
			return -1;
       else {
           fprintf(f_out, "invalid character @%d=_%c_\n", i, exp[i]);
           break;
       }
   }
   return 1;
}
 */

//handle *top = (handle) {.head = NULL, .memory = 0};
