#include "misc.h"
#include "operations.h"
#include "stack.h"

unsigned int ASRT_COUNT = 0;  // asrt() macro error count

int main(int argc, char** argv) {
    args config = parse_args(argc, argv);
    if (!(config.should_exit)) {
        read_text(config);
    }
    
    if (ASRT_COUNT) {
        fprintf(stderr, "\nerr_count = %u\n\tpress any key to exit", ASRT_COUNT);
        getchar();
    }

    return 0;
}
