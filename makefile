rpn: src/*
	gcc -std=c17 -Wall -Werror -Wextra -pedantic-errors src/* -lm -o rpn

clean:
	rm rpn