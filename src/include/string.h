/*
 * string.h
 *
 *  Created on: Oct 21, 2015
 *      Author: fagci
 */

#include <stddef.h>

#ifndef STRING_H_
#define STRING_H_

size_t strlen(const char* string);
size_t strcmp(const char *str1, const char *str2);
void itoa(char *buf, int base, int d);

#endif /* STRING_H_ */
