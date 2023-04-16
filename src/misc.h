#ifndef MISC_H
#define MISC_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"
#include "stack.h"

#define DEFAULT_ARGS          \
    {                         \
        .should_exit = false, \
        .infile = NULL,       \
        .outfile = NULL,      \
        .whitespace = ' ',    \
        .comment = '#',       \
        .quit = '$',          \
        .precision = 2,       \
        .deleter = 'x',       \
        .memory = 'm'         \
    }


#define EXP_LEN_MAX 1024					 	// maximum character length of a single RPN expression
#define RESTRICTED_CHARS "0123456789.+-*/^"  	
#define ERROR_MSG \
    "A critical error has occured.\n"

#define streq(str1, str2) !strcmp(str1, str2)  	// returns true if strings have equivalent values

extern unsigned int ASRT_COUNT;  				// initialized in main.c

/* error-catching macro */
#define asrt(z)                                                      \
    if (!z) {                                                        \
        ++ASRT_COUNT;                                                \
        fprintf(stderr, ERROR_MSG);                                  \
        fprintf(stderr, "error@ %s, line %d\n", __FILE__, __LINE__); \
        return 0;                                                    \
    }
//

/* command line arguments */
typedef struct args {
    int should_exit;  		// equals 0 if parsing succeeds
    char *infile;	 		// output file path
    char *outfile;			// input file path
    char whitespace;  		// whitespace character- ' ' by default
    char comment;	 		// comment initializer
    char quit;		  		// 'quit' character
    char precision;   		// decimal precision for output
    char deleter;	 		// stack-clearing character
    char memory;	  		// memory-operation character
} args;

/* prints help to stdout */
void get_help();

/** checks if file path is valid, allocates destination memory and copies the path
 * @param destination 
 * @param source path
 * @return true upon success
 * */
bool copy_path(char **destination, const char *path);

/* checks if args are not conflicting */
void verify_args(args* config, char* config_chars);

/** parses command line arguments
 * @return if parsing fails, the 'should_exit' field of the returned structure is set to true
 * */
args parse_args(int argc, char **argv);

/** allocates and initializes a stack handle
 * @return NULL upon failure
 * */
handle *new_stack();

/** checks if given strings contains a valid number and parses it
 * @param str string to be processed
 * @param parsed_number pointer to a double
 * @return true if the string contained a valid number, which is returned via the second argument
 * */
bool is_number(const char *str, double *parsed_number);

/** processes a string that contains an RNP expression
 * @param exp string to be processed
 * @param top initialized stack handle
 * @param config program config
 * @param f_out opened output stream
 * @return true = powodzenie
 * */
bool parse_exp(char *exp, handle *const top, const args config, FILE *f_out);

/* opens files based on config*/
void set_files(const args config, FILE** f_in, FILE** f_out);

/* closes files based on config */
void clear_files(const args* config, FILE** f_in, FILE** f_out);

/** handles the processing of RPN expressions
 * @param config program config
 * @return true upon success
 * */
bool read_text(const args config);

#endif  // MISC_H