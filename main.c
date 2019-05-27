#include "header.h"

#define debug(z)    \
	if (debug_mode) \
		fprintf(stderr, z);

//to do: xml

unsigned int asrt_count = 0, debug_mode = 0;

int main(int argc, char** argv) {
	args config = parse_args(argc, argv);
	if (!(config.should_exit)) {
		if (config.infile)
			debug(config.infile);
		if (config.outfile)
			debug(config.outfile);
		if (config.xmlfile)
			debug(config.xmlfile);
		if (debug_mode)
			fprintf(stderr, "whtspc=_%c_\ncmnt=%c\nquit=%c\n", config.whitespace, config.comment, config.quit);

		if (!read_text(config))
			debug("read_txt returned 0");
	}

	if (config.infile)
		free(config.infile);
	if (config.outfile)
		free(config.outfile);
	if (config.xmlfile)
		free(config.xmlfile);

	if (asrt_count) {
		fprintf(stderr, "err_count = %u\n", asrt_count);
		getchar();

		return 0;
	}
}
