#include "stdint.h"

enum color {
    BLACK = 0,
    BRIGHT = 7
};

enum size {
    COLS = 80,
    ROWS = 25
};

uint16_t *const video = (uint16_t*) 0xB8000;
uint16_t caret_position = 0;
enum color fg_color = BRIGHT, bg_color = BLACK;

void putc(uint8_t x, uint8_t y, enum color fg, enum color bg, char c) {
    video[y * COLS + x] = (bg << 12) | (fg << 8) | c;
}

char getc(uint8_t x, uint8_t y) {
    return video[y * COLS + x] & 0xff;
}

void puts(uint8_t x, uint8_t y, enum color fg, enum color bg, const char *s) {
    for (; *s; s++, x++)
        putc(x, y, fg, bg, *s);
}


void clear(enum color bg) {
    uint8_t x, y;
    caret_position = 0;
    for (y = 0; y < ROWS; y++)
        for (x = 0; x < COLS; x++)
            putc(x, y, bg, bg, ' ');
}

void scroll(){
    uint8_t x, y;
    for (y = 1; y < ROWS; y++)
	for (x = 0; x < COLS; x++)
	    if(y < ROWS -1) putc(x, y - 1, BRIGHT, BLACK, getc(x, y));
	    else putc(x, y, BRIGHT, BLACK, ' ');
}

void printf(const char *s) {
    puts(caret_position % ROWS, (int)(caret_position / ROWS), fg_color, bg_color, s);
    caret_position += sizeof(s) / sizeof(s[0]);
    if(caret_position > ROWS * COLS){
	caret_position -= ROWS * COLS;
	scroll();
    }
}


extern "C"
void kernel_main() {
    clear(BLACK);
    uint8_t i;
    for(i=0;i<200;i++) printf("hello world");
    while (1);
}
