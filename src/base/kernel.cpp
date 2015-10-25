#include <stdint.h>
#include "tty.h"
#include "vga.h"
#include "hal.h"
#include "ps2.h"
#include "stdio.h"
#include "stdlib.h"

static inline uint64_t rdtsc() {
    uint64_t ret;
    asm volatile ( "rdtsc" : "=A"(ret) );
    return ret;
}

extern "C" {
    void kernel_early(void) {
        tty_init();
        initGDT();
        printf("GDT initialized\n");
        initIDT();
        printf("IDT initialized\n");
        enableInterrupts();
        init_ps2();
    }
    void kernel_main(void) {
        tty_setfg(COLOR_GREEN);
        tty_moveto(0, 3);
        printf("---------\n");
        printf("FOS v0.01\n");
        printf("---------\n");
        printf("%i\t%d\t%s\t-\n", 42, 42, "test");
        printf("-\t--\t---\t----\n");
        idle();
    }
}
