#include <ctype.h>

#include "misc.h"

void get_help() {
    puts("Reverse Polish Notation Calculator");
    puts("author: Konfirm44");
    puts("");
    puts("available options:");
    puts("-i		input file directory - if no file is provided, stdin is used");
    puts("-o		output file directory - if no file is provided, stdout is used");
    puts("-w		set whitespace character 			- default: ' '");
    puts("-c		set comment character 				- default: '#'");
    puts("-q		set quit character 				- default: '$'");
    puts("-d		set stack-delete character 			- default: 'x'");
    puts("-m		set memory operation specifier character 	- default: 'm'");
    puts("-p		set output float precision 			- default: 2");
    puts("-h OR -?	displays help");
    puts("");
    puts("example: rpn.exe -i input.txt -o output.txt -w _ -c @ -q & -p 3");
}

bool copy_path(char** destination, const char* path) {
    if (strcspn(path, "<>|?*") != strlen(path)) {
        return 0;
    }
    
    (*destination) = malloc(strlen(path) + 1);
    asrt(*destination);
    strcpy(*destination, path);
    return 1;
}

void verify_args(args* config, char* config_chars) {
    if (!config->should_exit) {
        for (size_t x = 0; x < strlen(config_chars); ++x) {
            for (size_t j = 0; j < strlen(config_chars); ++j) {
                if (x != j && config_chars[x] == config_chars[j]) {
                    config->should_exit = true;
                    fprintf(stderr, "ERROR: configured characters must not be equal\n");
                    break;
                }
            }

            if (config->should_exit == true) {
                break;
            }
        }
    }
    else if (config->should_exit == -2) {
        fprintf(stderr, "ERROR: configured characters must not belong to this set '%s'\n", RESTRICTED_CHARS);
    }

    if (config->should_exit != 0) {
        if (config->infile) {
            free(config->infile);
        }
        if (config->outfile) {
            free(config->outfile);
        }
    }
}

/* single-char param parsing macro */
#define arg_char(struct_member)                                                        \
    if (strlen(argv[++i]) != 1) {                                                      \
        config.should_exit = true;                                                     \
    }                                                                                  \
    else {                                                                             \
        if (strchr(RESTRICTED_CHARS, argv[i][0])) {                                    \
            config.should_exit = -2;                                                   \
        }                                                                              \
        else {                                                                         \
            char* ptr = strchr(config_chars, config.struct_member);                    \
            if (!ptr) {                                                                \
                fprintf(stderr, "FATAL ERROR: arg_char _%c_\n", config.struct_member); \
                config.should_exit = true;                                             \
            }                                                                          \
            else {                                                                     \
                *ptr = argv[i][0];                                                     \
                config.struct_member = argv[i][0];                                     \
            }                                                                          \
        }                                                                              \
    }
//

args parse_args(int argc, char** argv) {
    args config = DEFAULT_ARGS;

    char config_chars[8] = "#$ xm";
    int i;
    for (i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (strlen(argv[i]) == 2) {
                char c = argv[i][1];
                if (i != argc - 1) {
                    if (c == 'i') {
                        if (!copy_path(&(config.infile), argv[++i])) {
                            config.should_exit = true;
                        }
                    }
                    else if (c == 'o') {
                        if (!copy_path(&(config.outfile), argv[++i])) {
                            config.should_exit = true;
                        }
                    }
                    else if (c == 'w') {
                        arg_char(whitespace);
                    }
                    else if (c == 'c') {
                        arg_char(comment);
                    }
                    else if (c == 'q') {
                        arg_char(quit);
                    }
                    else if (c == 'd') {
                        arg_char(deleter);
                    }
                    else if (c == 'm') {
                        arg_char(memory);
                    }
                    else if (c == 'p') {
                        char* endptr;
                        size_t t = strtol(argv[i], &endptr, 10);
                        if ((*endptr) != '\0' || t > 15) {
                            config.should_exit = true;
                        }
                        else {
                            config.precision = t;
                        }
                    }
                    else {
                        config.should_exit = true;
                    }
                }
                else if (c == 'h' || c == '?') {
                    get_help();
                    config.should_exit = -1;
                }
                else {
                    config.should_exit = true;
                }
            }
            else {
                config.should_exit = true;
            }
        }
        if (config.should_exit == true) {
            fprintf(stderr, "ERROR: invalid argument #%d:_%s_\n", i + 1, argv[i]);
            break;
        }
    }
    verify_args(&config, config_chars);
    return config;
}

bool is_number(const char* ptr, double* parsed_number) {
    char* endptr;
    double val = strtof(ptr, &endptr);
    if ((*endptr) == '\0') {
        *parsed_number = val;
        return 1;
    }
    return 0;
}

bool parse_exp(char* exp, handle* const top, const args config, FILE* f_out) {
    asrt(exp);
    if (config.infile) {
        fprintf(f_out, "exp= %s\n", exp);
    }
    char delim[2] = " ";
    delim[0] = config.whitespace;
    char* ptr = strtok(exp, delim);
    while (ptr) {
        double d;
        if (ptr[0] == config.comment) {
            break;
        }
        else if (is_number(ptr, &d)) {
            asrt(push(top, d));
        }
        else if (ptr[0] == config.memory) {
            if (!memory_operation(top, ptr)) {
                fprintf(f_out, "ERROR: invalid memory operator\n");
                pulverize(top);
                return 0;
            }
        }
        else if (ptr[0] == config.deleter) {
            pulverize(top);
        }
        else {
            const operation* op = get_operation(ptr);
            if (op) {
                if (top->stacksize >= op->num_of_operands) {
                    double* operands = get_operands(top, op->num_of_operands);
                    asrt(operands);
                    asrt(push(top, op->fn_ptr(operands)));
                    free(operands);
                }
                else {
                    fprintf(f_out, "ERROR: too few operands\n");
                    pulverize(top);
                    return 0;
                }
            }
            else {
                fprintf(f_out, "ERROR: invalid token '%s'\n", ptr);
                pulverize(top);
                return 0;
            }
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

void set_files(const args config, FILE** f_in, FILE** f_out) {
    if (config.infile) {
        *f_in = fopen(config.infile, "r");
    }
    else {
        fprintf(stderr, "WAITING FOR INPUT, '%c' to exit:\n", config.quit);
    }

    if (config.outfile) {
        *f_out = fopen(config.outfile, "w");
    }

    if (!(*f_out)) {
        *f_out = stdout;
        fprintf(stderr, "ERROR: could not open output file; all output will be directed to console\n");
    }
}

void clear_files(const args* config, FILE** f_in, FILE** f_out) {
    if (config->infile) {
        fclose(*f_in);
        free(config->infile);
    }
    if (config->outfile) {
        fclose(*f_out);
        free(config->outfile);
    }
}

bool read_text(const args config) {
    FILE* f_in = stdin, * f_out = stdout;
    set_files(config, &f_in, &f_out);

    if (f_in) {
        char exp[EXP_LEN_MAX];
        handle* top = new_stack();
        while (fgets(exp, EXP_LEN_MAX, f_in)) {
            if (exp[0] == config.quit) {
                break;
            }

            if (exp[0] == '\n' || exp[0] == config.comment) {
                continue;
            }

            if (exp[strlen(exp) - 1] == '\n') {
                exp[strlen(exp) - 1] = '\0';
            }

            if (!parse_exp(exp, top, config, f_out)) {
                fprintf(f_out, "ERROR: expression invalid\n");
            }
            else {
                if (top->head) {
                    double d;
                    if (!pop(top, &d)) {
                        fprintf(f_out, "FATAL ERROR: could not pop\n");
                    }
                    else {
                        fprintf(f_out, "\n\t= %.*f\n", config.precision, d);
                    }
                }
                else {
                    fprintf(f_out, "\n\t= ___\nCalculator memory appears to be empty. Perhaps you used the '%c' operator at the end of your expression?\n", config.deleter);
                }
            }
        }
        pulverize(top);
        free(top);
        clear_files(&config, &f_in, &f_out);
        return 1;
    }
    else {
        fprintf(stderr, "ERROR: could not open input file; program will now exit\n");
    }

    return 0;
}
