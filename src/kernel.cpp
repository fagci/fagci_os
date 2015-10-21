#include <stdint.h>
#include "klib/tty.h"
#include "klib/vga.h"
#include "klib/stdio.h"

extern "C" {
    void kernel_early(void) {
        tty_init();
    }
    void kernel_main(void) {
        for (size_t i = 0; i < 2000; i++){
            printf("Hello world! %d\n", i);
            tty_setfg((enum vga_color)(i%15+1));
        }

        while (1);
    }
}
