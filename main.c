/** @file */
/** @author Tomasz Sitek */
#include "misc.h"
#include "operations.h"
#include "stack.h"

unsigned int ASRT_COUNT = 0; // liczba wystąpień błędów, nieistotne dla użytkownika

int main(int argc, char** argv) {
	args config = parse_args(argc, argv);
	if (!(config.should_exit)) 
		read_text(config);

	if (config.infile)
		free(config.infile);
	if (config.outfile)
		free(config.outfile);

	if (ASRT_COUNT) {
		fprintf(stderr, "\nerr_count = %u\n\tpress any key to exit", ASRT_COUNT);
		getchar();
	}
	
	return 0;
}
