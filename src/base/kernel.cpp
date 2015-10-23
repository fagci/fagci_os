#include <stdint.h>
#include "tty.h"
#include "vga.h"
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
    }
    void kernel_main(void) {
        tty_clear(COLOR_LIGHT_CYAN);
        tty_setfg(COLOR_GREEN);
        tty_moveto(0, 3);
        printf("---------\n");
        printf("FOS v0.01\n");
        printf("---------\n");
        tty_setbg(COLOR_RED);
        tty_setfg(COLOR_WHITE);
        printf("Testing functionality...");
        CaretEntry* ce = tty_getentry();
        static char const spin_chars[] = "/-\\|";
        for(size_t i = 0; i<2000000; i++){
            tty_moveto(ce->x, ce->y);
            tty_putc(spin_chars[i%4]);
        }

        for(;;) {
            asm("hlt");
        }
    }
}
