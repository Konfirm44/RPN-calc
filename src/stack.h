#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

/** stack element */
typedef struct stack {
    double value;
    struct stack* next;
} stack;

/** stack handle */
typedef struct handle {
    stack* head;
    double memory;
    unsigned int stacksize;
} handle;

/** adds a number on top of the stack
 * @param top stack handle
 * @param d the number
 * @return true upon success
 * */
bool push(handle* const top, const double d);

/** pops and returns the number from the top of the stack
 * @param top stack handle
 * @param d return-pointer
 * @return true upon success
 * */
bool pop(handle* const top, double* d);

/** returns the topmost number without popping it
 * @param top stack handle
 * @return value of the number or 0 if the stack is empty
 * */
double peek(handle* const top);

/** destroys the stack
 * @param top stack handle
 * */
void pulverize(handle* const top);

#endif  // STACK_H