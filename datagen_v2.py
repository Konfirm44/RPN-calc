#!/usr/bin/env python3
"""Generator danych do projektu."""

from random import randint, choice, uniform
from itertools import combinations

LICZBA_PRZYKLADOW = 10
MIN_UNIFORM = -256
MAX_UNIFORM = 256
MIN_N = 1
MAX_N = 2

OPERATORY = [("+", lambda a, b : a + b),
             ("-", lambda a, b : a - b),
             ("*", lambda a, b : a * b),
             ("/", lambda a, b : a / b)]

def zloz_dzialanie(n):
    if n > 0:
        a = zloz_dzialanie(n - randint(1, n))
        b = zloz_dzialanie(n - randint(1, n))
        op = choice(OPERATORY)
        return ("{} {} {}".format(a[0], b[0], op[0]),
                op[1](a[1], b[1]))
    else:
        a = uniform(MIN_UNIFORM, MAX_UNIFORM)
        return ("{:.2f}".format(a), a)

for i in range(LICZBA_PRZYKLADOW):
    expr = zloz_dzialanie(randint(MIN_N, MAX_N))
    print("{} # == {:.2f}".format(expr[0], expr[1]))