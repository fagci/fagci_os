#include "stdint.h"
#include "klib/tty.h"
#include "klib/vga.h"
#include "klib/serial.h"
#include "klib/string.h"

/*
void update_cursor(int row, int col) {
    unsigned short position=(row*80) + col;
 
    // cursor LOW port to vga INDEX register
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position&0xFF));
    // cursor HIGH port to vga INDEX register
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char )((position>>8)&0xFF));
}
*/
void printf(const char *s) {
	tty_puts(s);
}

extern "C" {
    void kernel_early(void) {
        init_serial(COM1);
        //write_serial(COM1, '1');
        //tty_init();
    }
    void kernel_main(void) {

        //
        /*size_t i;
        for (i = 0; i < VGA_HEIGHT / 2; i++)
            printf("hello world!\n");

        const char *w = "Welcome to fagci's OS!";
        printf(w);*/
        while (1);
    }
}
