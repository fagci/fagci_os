#include "vga.h"

#ifndef TTY_H_
#define TTY_H_

void tty_init(void);

void tty_setbg(enum vga_color c);
void tty_setfg(enum vga_color c);

struct CaretEntry {
    CaretEntry(uint16_t cx, uint16_t cy) {
        this -> x = cx;
        this -> y = cy;
    }
    uint16_t x;
    uint16_t y;
};

CaretEntry* tty_getentry();

void tty_scroll();
void tty_clear(enum vga_color bg);
void tty_moveto(uint16_t x, uint16_t y);
void tty_putentryat(uint16_t x, uint16_t y, enum vga_color fg, enum vga_color bg, char c);

void tty_putc(char c);
void tty_puts(const char *s);
void tty_write(const char* s, size_t size);


#endif /* TTY_H_ */
