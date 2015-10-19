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
uint16_t caret_x = 0, caret_y = 0;
enum color fg_color = BRIGHT, bg_color = BLACK;

void* memmove(void* dstptr, const void* srcptr, uint16_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;
    if (dst < src)
	for (uint16_t i = 0; i < size; i++)
	    dst[i] = src[i];
    else
	for (uint16_t i = size; i != 0; i--)
	    dst[i-1] = src[i-1];
    return dstptr;
}

void put_entry_at(uint8_t x, uint8_t y, enum color fg, enum color bg, char c) {
    video[y * COLS + x] = (bg << 12) | (fg << 8) | c;
}

void scroll() {
    uint8_t x;
    memmove(video, video + COLS, COLS * (ROWS - 1));
    for (x = 0; x < COLS; x++)
	put_entry_at(x,caret_y,fg_color,bg_color,' ');
}

void putc(char c) {
    if(c == '\n'){
	caret_x = 0;
	caret_y++;
	return;
    }
    put_entry_at(caret_x, caret_y, fg_color, bg_color, c);
    if(++caret_x == COLS){
	caret_x = 0;
	if(++caret_y == ROWS){
	    scroll();
	    caret_y--;
	}
    }
}

uint16_t strlen(const char* string)
{
    uint16_t result = 0;
    while ( string[result] )
	result++;
    return result;
}

void write(const char* s, uint16_t size){
    for(uint16_t i = 0; i < size; i++)
	putc(s[i]);
}

void puts(const char *s) {
    write(s, strlen(s));
}


void clear(enum color bg) {
    uint8_t x, y;
    caret_x = caret_y = 0;
    for (y = 0; y < ROWS; y++)
        for (x = 0; x < COLS; x++)
            put_entry_at(x, y, fg_color, bg_color, ' ');
}

void printf(const char *s) {
    puts(s);
}

extern "C" {
    void kernel_early(){}
    void _init(){}
    void kernel_main() {
	clear(BLACK);
	uint8_t i;
	for(i=0;i<200;i++) printf("hello world\n"),fg_color = (enum color)(i%7+1);
///printf("hello world!");
///memmove(video + COLS, video, COLS);
while (1);
    }
}
