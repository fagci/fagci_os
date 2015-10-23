#include <stdarg.h>
#include "tty.h"

char* convert(unsigned int num, int base) {
    static char buff[33];
    char *ptr;
    ptr = &buff[sizeof(buff) - 1];
    *ptr = '\0';
    do {
        *--ptr = "0123456789abcdef"[num % base];
        num /= base;
    } while (num != 0);
    return (ptr);
}

void printf(const char * frmt, ...) {
    const char *p;
    int i;
    unsigned u;
    char *s;
    va_list argp;
    va_start(argp, frmt);
    p = frmt;
    for (p = frmt; *p != '\0'; p++) {
        if (*p != '%') {
            tty_putc(*p);
            continue;
        }
        p++;
        switch (*p) {
            case 'c':
                i = va_arg(argp,int);
                tty_putc(i);
                break;
            case 'd':
                i = va_arg(argp,int);
                if (i < 0) {
                    i = -i;
                    tty_putc('-');
                }
                tty_puts(convert(i, 10));
                break;
            case 'o':
                i = va_arg(argp,unsigned int);
                tty_puts(convert(i, 8));
                break;
            case 's':
                s = va_arg(argp,char *);
                tty_puts(s);
                break;
            case 'u':
                u = va_arg(argp, unsigned int);
                tty_puts(convert(u, 10));
                break;
            case 'x':
                u = va_arg(argp, unsigned int);
                tty_puts(convert(u, 16));
                break;
            case '%':
                tty_putc('%');
                break;
        }
    }
    va_end(argp);
}

