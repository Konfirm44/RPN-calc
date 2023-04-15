# RPN-calc

A console-based [Reverse Polish Notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation) calculator written in C.

## Setup

Compile with gcc using `make`, then run the `rpn` executable.

Run `rpn -h` or `rpn -?` for help.

## Features

The calculator supports the 4 basic floating-point (double precision) operations, as well as exponentiation: `+, -, *, /, ^`. The number of decimal digits in the output can be modified with the `-p` option (2 by default). Memory operations can be performed using `m+, m-, mr, mc` operators (the `m` character can be substituted using the `-m` option).

By default, the calculator works with stdin & stdout. It is able to process file input & output, which can be configured using the `-i, -o` options. When the `#` character is found in the input (can be modified with the `-c` option), processing of the current line is finished, which allows for comments when working with files. The `$` character (modifiable via `-q`) can be used to quit the app.

For no good reason, the calculator can be configured to treat a character other than space as whitespace. To do that, use the `-w` option. To add to the insanity, `x` can be used mid-calculations to force-clear the stack (likewise modifiable with `-d`).
