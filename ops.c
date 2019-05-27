#include "header.h"
#include <math.h>

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
