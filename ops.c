#include <math.h>

#include "header.h"

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

bool push(handle* const top, const double d) {
	stack* p = malloc(sizeof(stack));
	asrt(p);
	p->value = d;
	p->next = top->head;
	top->head = p;
	++(top->stacksize);
	return 1;
}

bool pop(handle* const top, double* d) {
	asrt(top->head);
	*d = top->head->value;
	stack* p = top->head->next;
	free(top->head);
	top->head = p;
	--(top->stacksize);
	return 1;
}

double peek(handle* const top) {
	if (!(top->head))
		return 0;
	return top->head->value;
}

void pulverize(handle* const top) {
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