#include "stdint.h"
#include "klib/tty.h"
#include "klib/vga.h"
#include "klib/serial.h"
#include "klib/string.h"

void printf(const char *s) {
	tty_puts(s);
}

extern "C" {
    void kernel_early(void) {
        init_serial(COM1);
        write_serial(COM1, '1');
        tty_init();
    }
    void kernel_main(void) {
        size_t i;
        for (i = 0; i < VGA_HEIGHT / 2; i++)
            printf("hello world!\n");

        const char *w = "Welcome to fagci's OS!";
        printf(w);
        while (1);
    }
}
