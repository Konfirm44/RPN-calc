/** @file */
#include <math.h>

#include "misc.h"
#include "operations.h"

double op_add(const double operands[2]) {
	return operands[1] + operands[0];
}

double op_subtract(const double operands[2]) {
	return operands[1] - operands[0];
}

double op_multiply(const double operands[2]) {
	return operands[1] * operands[0];
}

double op_divide(const double operands[2]) {
	return (operands[0] == 0) ? 0 : operands[1] / operands[0];
}

double op_pow(const double operands[2]) { 
	return pow(operands[1], operands[0]);
}

bool memory_operation(handle* const top, const char* op) {
	if (eq(op, "m+")) {
		top->memory += peek(top);
		return 1;
	} else if (eq(op, "m-")) {
		top->memory -= peek(top);
		return 1;
	} else if (eq(op, "mr")) {
		push(top, top->memory);
		return 1;
	} else if (eq(op, "mc")) {
		top->memory = 0;
		return 1;
	}
	return 0;
}

const operation* get_operation(const char* str) {
	static const operation ops[n_operations] = {
		{op_add, "+", 2},
		{op_subtract, "-", 2},
		{op_multiply, "*", 2},
		{op_divide, "/", 2},
		{op_pow, "^", 2}};
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
		asrt(pop(top, operands + i));
		if (!(top->head) && (i != num_of_operands - 1)) {
			free(operands);
			return NULL;
		}
	}
	return operands;
}
