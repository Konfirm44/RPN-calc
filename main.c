/** @file */
/** @author Tomasz Sitek */
#include "misc.h"
#include "operations.h"
#include "stack.h"

unsigned int asrt_count = 0; // liczba wystąpień błędów, nieistotne dla użytkownika

int main(int argc, char** argv) {
	args config = parse_args(argc, argv);
	if (!(config.should_exit)) 
		read_text(config);

	if (config.infile)
		free(config.infile);
	if (config.outfile)
		free(config.outfile);

	if (asrt_count) {
		fprintf(stderr, "err_count = %u\n press any key to exit", asrt_count);
		getchar();
	}
	
	return 0;
}
