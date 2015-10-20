#include "tty.h"
#include "vga.h"
#include "string.h"
#include "memory.h"

#include "serial.h"

uint16_t* tty_buffer;
size_t tty_x, tty_y;
enum vga_color fg_color = COLOR_WHITE, bg_color = COLOR_BLACK;

void tty_init(void)
{
    write_serial(COM1,'2');
    tty_x = 0;
    tty_y = 0;
    tty_buffer = VGA_MEMORY;
    for ( size_t y = 0; y < VGA_HEIGHT; y++ )
    {
        for ( size_t x = 0; x < VGA_WIDTH; x++ )
        {
            const size_t index = y * VGA_WIDTH + x;
            tty_buffer[index] = make_vgaentry(' ', bg_color);
        }
    }
}

void tty_putentryat(size_t x, size_t y, enum vga_color fg, enum vga_color bg, char c) {
    tty_buffer[y * VGA_WIDTH + x] = (bg << 12) | (fg << 8) | c;
}

void tty_scroll() {
    size_t x;
    memmove(tty_buffer, tty_buffer + VGA_WIDTH, VGA_WIDTH * (VGA_HEIGHT - 1));
    for (x = 0; x < VGA_WIDTH; x++)
        tty_putentryat(x, tty_y, fg_color, bg_color, ' ');
}

void tty_putc(char c) {
    if (c == '\n') {
        tty_x = 0;
        tty_y++;
        return;
    }
    tty_putentryat(tty_x, tty_y, fg_color, bg_color, c);
    //update_cursor(caret_x, caret_y);
    if (++tty_x == VGA_WIDTH) {
        tty_x = 0;
        if (++tty_y == VGA_HEIGHT) {
            tty_scroll();
            tty_y--;
        }
    }
}

void tty_write(const char* s, size_t size) {
    for (size_t i = 0; i < size; i++)
        tty_putc(s[i]);
}

void tty_puts(const char *s) {
    tty_write(s, strlen(s));
}

void tty_clear(enum vga_color bg) {
    size_t x, y;
    tty_x = tty_y = 0;
    for (y = 0; y < VGA_HEIGHT; y++)
        for (x = 0; x < VGA_WIDTH; x++);
            tty_putentryat(x, y, fg_color, bg, ' ');
}
