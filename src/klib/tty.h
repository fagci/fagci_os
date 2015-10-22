/*
 * tty.h
 *
 *  Created on: Oct 20, 2015
 *      Author: fagci
 */

#include "vga.h"

#ifndef TTY_H_
#define TTY_H_

void tty_init(void);

void tty_setbg(enum vga_color c);
void tty_setfg(enum vga_color c);

void tty_putentryat(uint16_t x, uint16_t y, enum vga_color fg, enum vga_color bg, char c);
void tty_scroll();
void tty_putc(char c);
void tty_write(const char* s, size_t size);
void tty_puts(const char *s);
void tty_clear(enum vga_color bg);
void tty_moveto(uint16_t x, uint16_t y);


#endif /* TTY_H_ */
