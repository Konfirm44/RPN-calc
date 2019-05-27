#include <stdlib.h>
#include <string.h>

#include "header.h"

//to do: function pointers, xml, help()

const operation* get_operation(const char* str) {
	static const operation ops[n_operations] = {
		{&op_add, "+", 2},
		{&op_subtract, "-", 2},
		{&op_multiply, "*", 2},
		{&op_divide, "/", 2},
		{&op_pow, "^", 2}
	};
	for (int i = 0; i < n_operations; ++i)
		if (!strcmp(ops[i].tag, str))
			return &(ops[i]);
	return NULL;
}

int temp_subsolve(handle* const top, double* a, double* b) {
	asrt(top->head);
	asrt(pop(top, b));
	if (!(top->head)) {
		push(top, *b);
		return 1;
	}
	asrt(pop(top, a));
	return 1;
}

int parse_exp_fp(char* exp, handle* const top, const args xd, FILE* f_out) {
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

		} else if (strlen(ptr) == 2 && (ptr[0] == 'm' || ptr[0] == 'M')) {  //może case na samo "m"
			asrt(memory_operation(top, ptr[1]));
		} else if (strlen(ptr) == 1) {
			if (ptr[0] == 'x' || ptr[0] == 'X') {
				pulverize(top);
			} else if (ptr[0] == 'm' || ptr[0] == 'M') {
				fprintf(f_out, "ERROR: no memory operation specifier; did you mean 'M+'?\n");
				return 0;
			} else {
				const operation* op = get_operation(ptr);
				if (op) {
					if (top->stacksize >= op->num_of_operands) {
						//for(num_of_operands) pop
						double a, b;
						temp_subsolve(top, &a, &b);
						d = op->fn_ptr(a, b);
						asrt(push(top, d));
					} else {
						fprintf(f_out, "ERROR: too few operands\n");
						return 0;
					}
				} else {
					fprintf(f_out, "ERROR: invalid operator '%s'\n", ptr);
					return 0;
				}
			}
		} else {
			fprintf(f_out, "ERROR: invalid token '%s'\n", ptr);
			return 0;
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

int read_text(const args xd) {
	FILE *f_in = stdin, *f_out = stdout;
	if (xd.infile)
		f_in = fopen(xd.infile, "r");
	else
		fprintf(stderr, "WAITING FOR INPUT, '$' to exit:\n");

	if (xd.outfile)
		f_out = fopen(xd.outfile, "w");
	if (!f_out) {
		f_out = stdout;
		fprintf(stderr, "ERROR: could not open output file; all output will be directed to console\n");
	}

	if (f_in) {
		char exp[exp_len_max];
		handle* top = new_stack();
		while (fgets(exp, exp_len_max, f_in)) {
			if (exp[0] == xd.abandon)
				break;
			if (exp[0] == '\n' || exp[0] == xd.comment)
				continue;
			for (int i = 0; i < strlen(exp); ++i)
				if (exp[i] == '\n')
					exp[i] = '\0';

			if (!parse_exp_fp(exp, top, xd, f_out)) {
				fprintf(f_out, "ERROR: expression invalid\n");
			} else {
				if (top->head){
					double d;
					if (!pop(top, &d))
						fprintf(f_out, "FATAL ERROR: could not pop\n");  //trza inny komunikat
					else
						fprintf(f_out, "\n\t= %.*f\n", xd.precision, d);
				} else {
					fprintf(f_out, "\n\t= ___\nCalculator memory appears to be empty. Perhaps you used the 'X' operator at the end of your expression?\n");
				}
			}
		}
		pulverize(top);
		free(top);
		if (xd.infile)
			fclose(f_in);
		if (xd.outfile)
			fclose(f_out);
		return 1;
	} else
		fprintf(stderr, "ERROR: could not open input file; program will now exit\n");
	return 0;
}

int main(int argc, char** argv) {
	args xd = parse_args(argc, argv);
	if (!(xd.should_exit)) {
		if (xd.infile)
			puts(xd.infile);
		if (xd.outfile)
			puts(xd.outfile);
		if (xd.xmlfile)
			puts(xd.xmlfile);
		printf("whtspc=_%c_\ncmnt=%c\nabndn=%c\n", xd.whitespace, xd.comment, xd.abandon);
	} else
		puts("no args");

	if (!read_text(xd))
		puts("read_txt returned 0");

	if (xd.infile)
		free(xd.infile);
	if (xd.outfile)
		free(xd.outfile);
	if (xd.xmlfile)
		free(xd.xmlfile);

	return 0;
}

/*
while (ptr) {
		double d;
		operacja* op;
		if (ptr[0] == xd.comment) {
			break;
		} else if (is_number(ptr, &d)) {
			asrt(push(top, d));
		} else if ((op = dopasuj_operacje(ptr)) != NULL) {
			if (top->stacksize >= op->operands) {
				asrt(op->fn_ptr(top));
			} else {
				fprintf(stderr, "ERROR: too few operands\n");
				return 0;
			}
		} else {
			fprintf(stderr, "ERROR: invalid token '%s'\n", ptr);
			return 0;
		}
		ptr = strtok(NULL, delim);
	}
*/
