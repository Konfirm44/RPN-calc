#include <ctype.h>
#include <string.h>

#include "header.h"

void get_help() {
	puts("REVERSE POLISH NOTATION CALCULATOR");
	puts("author: Tomasz Sitek");
	puts("");
	puts("available options:");
	puts("-i		input file directory - if no file is provided, stdin is used");
	puts("-o		output file directory - if no file is provided, stdout is used");
	puts("-x		xml input file directory - NOT SUPPORTED IN CURRENT VERSION");
	puts("-w		set whitespace character - default: ' '");
	puts("-c		set comment character - default: '#'");
	puts("-q		set quit character - default: '$'");
	puts("-p		set output float precision - default: 2");
	puts("-h OR -?	displays help");
	puts("");
	puts("example: onp.exe -i input.txt -o output.txt -w _ -c @ -q & -p 3");
	puts("");
	puts("The program will now exit.");

	/* 
	i	input
	o	output
	x	xml
	w	whitespace
	c	comment
	q	quit
	p	precision
	h	help 
	*/
}

bool copy_string(char** destination, const char* source) {
	if (strcspn(source, "<>:\"/\\|?*") != strlen(source))
		return 0;
	(*destination) = malloc(strlen(source) + 1);
	asrt(*destination);
	strcpy(*destination, source);
	return 1;
}

args parse_args(int argc, char** argv) {
	args config = {
		.should_exit = false,
		.infile = NULL,
		.outfile = NULL,
		.xmlfile = NULL,
		.whitespace = ' ',
		.comment = '#',
		.quit = '$',
		.precision = 2};

	int i;
	for (i = 1; i < argc; ++i) {
		if (argv(i, 0) == '-') {
			if (strlen(argv[i]) == 2) {
				char c = argv(i, 1);
				if (c == 'i') {
					if (i == argc - 1 || !copy_string(&(config.infile), argv[++i]))
						config.should_exit = true;
				} else if (c == 'o') {
					if (i == argc - 1 || !copy_string(&(config.outfile), argv[++i]))
						config.should_exit = true;
				} else if (c == 'x') {
					if (i == argc - 1 || !copy_string(&(config.xmlfile), argv[++i]))
						config.should_exit = true;
				} else if (c == 'w') {
					if (i == argc - 1 || strlen(argv[++i]) != 1) {
						config.should_exit = true;
					} else {
						config.whitespace = argv(i, 0);
						if (strchr(restricted_characters, config.whitespace))
							config.should_exit = true;
					}
				} else if (c == 'c') {
					if (i == argc - 1 || strlen(argv[++i]) != 1) {
						config.should_exit = true;
					} else {
						config.comment = argv(i, 0);
						if (strchr(restricted_characters, config.comment))
							config.should_exit = true;
					}
				} else if (c == 'q') {
					if (i == argc - 1 || strlen(argv[++i]) != 1) {
						config.should_exit = true;
					} else {
						config.quit = argv(i, 0);
						if (strchr(restricted_characters, config.quit))
							config.should_exit = true;
					}
				} else if (c == 'p') {
					if (i == argc - 1 || strlen(argv[++i]) != 1) {
						config.should_exit = true;
					} else {
						char* endptr;
						size_t t = strtol(argv[i], &endptr, 10);
						if ((*endptr) != '\0' || t < 0 || t > 15)
							config.should_exit = true;
						else
							config.precision = t;
					}
				} else if (c == 'h' || c == '?') {
					get_help();
					config.should_exit = -1;
				} else
					config.should_exit = true;
			} else
				config.should_exit = true;
		}
		if (config.should_exit == true) {
			fprintf(stderr, "ERROR: invalid argument #%d:_%s_\n", i + 1, argv[i]);
			break;
		}
	}
	
	if (config.comment == config.whitespace || config.comment == config.quit || config.quit == config.whitespace) {
		config.should_exit = true;
		fprintf(stderr, "ERROR: comment/quit/whitespace characters must not be equal\n");
	}

	return config;
}

handle* new_stack() {
	handle* top = malloc(sizeof(handle));
	top->head = NULL;
	top->memory = 0;
	top->stacksize = 0;
	return top;
}

bool is_number(const char* ptr, double* d) {
	char* endptr;
	double val = strtof(ptr, &endptr);
	if ((*endptr) == '\0') {
		*d = val;
		return 1;
	}
	return 0;
}

bool memory_operation(handle* const top, const char op) {
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
		return 0;
	}
}

const operation* get_operation(const char* str) {
	static const operation ops[n_operations] = {
		{op_add, "+", 2},
		{op_subtract, "-", 2},
		{op_multiply, "*", 2},
		{op_divide, "/", 2},
		{op_pow, "^", 2}
	};
	for (int i = 0; i < n_operations; ++i)
		if (!strcmp(ops[i].tag, str))
			return &(ops[i]);
	return NULL;
}

double* get_operands(handle* const top, unsigned int num_of_operands) {
	asrt(top->head);
	double* operands = malloc(num_of_operands * sizeof(double));
	asrt(operands);
	for (int i = 0; i < num_of_operands; ++i) {
		asrt(pop(top, operands+i));
		if (!(top->head) && (i != num_of_operands-1)) {
			free(operands);
			return NULL;
		}
	}
	return operands;
}

bool parse_exp(char* exp, handle* const top, const args config, FILE* f_out) {
	asrt(exp);
	if (config.infile)
		fprintf(f_out, "exp= %s\n", exp);

	char delim[2] = " ";
	delim[0] = config.whitespace;
	char* ptr = strtok(exp, delim);
	while (ptr) {
		double d;
		if (ptr[0] == config.comment) {
			break;
		} else if (is_number(ptr, &d)) {
			asrt(push(top, d));
		} else if (ptr[0] == 'm' || ptr[0] == 'M') {  
			if (!memory_operation(top, ptr[1])) {
				fprintf(f_out, "ERROR: invalid memory operator\n");
				return 0;
			}
		} else {
			if (ptr[0] == 'x' || ptr[0] == 'X') {
				pulverize(top);
			} else if (ptr[0] == 'm' || ptr[0] == 'M') {
				fprintf(f_out, "ERROR: no memory operation specifier; did you mean 'M+'?\n");
				return 0;
			} else {
				const operation* op = get_operation(ptr);
				if (op) {
					if (top->stacksize >= op->num_of_operands) {
						double* operands = get_operands(top, op->num_of_operands);
						asrt(operands);
						asrt(push(top, op->fn_ptr(operands))); 
						free(operands);
					} else {
						fprintf(f_out, "ERROR: too few operands\n");
						return 0;
					}
				} else {
					fprintf(f_out, "ERROR: invalid token '%s'\n", ptr);
					return 0;
				}
			}
		}
		ptr = strtok(NULL, delim);
	}

	if (top->stacksize > 1) {
		fprintf(f_out, "ERROR: not enough operators; %u operator(s) missing\n", (top->stacksize) - 1);
		pulverize(top);
		return 0;
	}
	
	return 1;
}

bool read_text(const args config) {
	FILE *f_in = stdin, *f_out = stdout;
	if (config.infile)
		f_in = fopen(config.infile, "r");
	else
		fprintf(stderr, "WAITING FOR INPUT, '%c' to exit:\n", config.quit);

	if (config.outfile)
		f_out = fopen(config.outfile, "w");
	if (!f_out) {
		f_out = stdout;
		fprintf(stderr, "ERROR: could not open output file; all output will be directed to console\n");
	}

	if (f_in) {
		char exp[exp_len_max];
		handle* top = new_stack();
		while (fgets(exp, exp_len_max, f_in)) {
			if (exp[0] == config.quit)
				break;
			if (exp[0] == '\n' || exp[0] == config.comment)
				continue;
			for (int i = 0; i < strlen(exp); ++i)
				if (exp[i] == '\n')
					exp[i] = '\0';

			if (!parse_exp(exp, top, config, f_out)) {
				fprintf(f_out, "ERROR: expression invalid\n");
			} else {
				if (top->head){
					double d;
					if (!pop(top, &d))
						fprintf(f_out, "FATAL ERROR: could not pop\n");  //trza inny komunikat
					else
						fprintf(f_out, "\n\t= %.*f\n", config.precision, d);
				} else {
					fprintf(f_out, "\n\t= ___\nCalculator memory appears to be empty. Perhaps you used the 'X' operator at the end of your expression?\n");
				}
			}
		}
		pulverize(top);
		free(top);
		if (config.infile)
			fclose(f_in);
		if (config.outfile)
			fclose(f_out);
		return 1;
	} else
		fprintf(stderr, "ERROR: could not open input file; program will now exit\n");

	return 0;
}
