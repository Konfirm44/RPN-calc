#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "stack.h"

#define N_OPERATIONS 5  	// size of the function pointers array

/** function pointers array element */
typedef struct operation {
	double (*fn_ptr)(const double operands[]);
	const char* tag;
	unsigned int num_of_operands;
} operation;

/** function-declaring macro 
 * @param name function name
 * */
#define op_function_declr(name) op_##name(const double operands[])

/** addition */
double op_function_declr(add);

/** subtraction */
double op_function_declr(subtract);

/** multiplication */
double op_function_declr(multiply);

/** floating-point division */
double op_function_declr(divide);

/** exponentation */
double op_function_declr(pow);

/** processes a memory operation according to the provided memory operator 
 * @param top stack handle
 * @param op string containing the memory operator
 * @return true upon success
 * */
bool memory_operation(handle* const top, const char* op);

/** returns function pointer based on the provided operator
 * @param str operator
 * @return NULL if the operator does not correspond to any available operation
 * */
const operation* get_operation(const char* str);

/** returns an operand array
 * @param top stack handle
 * @param num_of_operands size of the array
 * @return pointer to the array
 * */
double* get_operands(handle* const top, unsigned int num_of_operands);

#endif  // OPERATIONS_H