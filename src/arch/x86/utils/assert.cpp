#include "hal.h"
#include "stdio.h"

void assert(const char *asserted_expr, const char *filename, const char *func, int line) {
	printf("\nFailed assert at %s, %s:%d\n %s", filename, func, line, asserted_expr);
	disable_interrupts();
    idle();
}
