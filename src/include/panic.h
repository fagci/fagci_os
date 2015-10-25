#ifndef __PANIC_H
#define __PANIC_H

void panic(const char *message, const char *filename, const char *func, int line);
#define PANIC(str) panic(str, __FILE__, __func__, __LINE__)

#endif