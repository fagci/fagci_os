#include "tty.h"
#include "vga.h"
#include "string.h"
#include "memory.h"

#include "hal.h"
#include "stdio.h"

uint16_t* tty_buffer;
uint16_t tty_x, tty_y;
enum vga_color fg_color = COLOR_WHITE, bg_color = COLOR_BLACK;



void tty_setbg(enum vga_color c) {
    bg_color = c;
}

void tty_setfg(enum vga_color c) {
    fg_color = c;
}

void tty_moveto(uint16_t x, uint16_t y) {
    tty_x = x;
    tty_y = y;
}

CaretEntry* tty_getentry() {
    return new CaretEntry(tty_x, tty_y);
}

void update_cursor(void) {
    unsigned short position = (tty_y * VGA_WIDTH) + tty_x;
    // cursor LOW port to vga INDEX register
    outportb(0x3D4, 0x0F);
    outportb(0x3D5, (unsigned char) (position & 0xFF));
    // cursor HIGH port to vga INDEX register
    outportb(0x3D4, 0x0E);
    outportb(0x3D5, (unsigned char) ((position >> 8) & 0xFF));
}

void tty_init(void) {
    tty_x = 0;
    tty_y = 0;
    tty_buffer = VGA_MEMORY;
    for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint16_t x = 0; x < VGA_WIDTH; x++) {
            const uint16_t index = y * VGA_WIDTH + x;
            tty_buffer[index] = make_vgaentry(' ', bg_color);
        }
    }
}

void tty_putentryat(uint16_t x, uint16_t y, enum vga_color fg, enum vga_color bg, char c) {
    tty_buffer[y * VGA_WIDTH + x] = (bg << 12) | (fg << 8) | c;
}

void tty_scroll() {
    uint16_t last_y = VGA_HEIGHT - 1;
    size_t maxlen = VGA_HEIGHT * VGA_WIDTH;
    for (uint16_t i = VGA_WIDTH; i < maxlen; i++)
        tty_buffer[i - VGA_WIDTH] = tty_buffer[i];
    for (uint16_t x = 0; x < VGA_WIDTH; x++)
        tty_putentryat(x, last_y, fg_color, bg_color, ' ');
    tty_y = last_y;
}

void tty_putc(char c) {
    switch (c) {
        case '\t':
            tty_x = (tty_x + 8) & ~(8 - 1);
            break;
        case '\n':
            tty_x = 0;
            tty_y++;
            break;
        case '\r':
            tty_x = 0;
            break;
        default:
            tty_putentryat(tty_x, tty_y, fg_color, bg_color, c);
            tty_x++;
            break;
    }

    if (tty_x == VGA_WIDTH) {
        tty_x = 0;
        if (tty_y == VGA_HEIGHT) tty_scroll();
    }


    update_cursor();
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
    bg_color = bg;
    tty_x = tty_y = 0;
    for (y = 0; y < VGA_HEIGHT; y++)
        for (x = 0; x < VGA_WIDTH; x++)
            tty_putentryat(x, y, fg_color, bg, ' ');
}
