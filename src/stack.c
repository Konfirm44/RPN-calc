#include "stack.h"
#include "misc.h"

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
