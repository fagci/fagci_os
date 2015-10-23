/*
 * memory.h
 *
 *  Created on: Oct 20, 2015
 *      Author: fagci
 */

#include <stddef.h>

#ifndef MEMORY_H_
#define MEMORY_H_

void* memmove(void* dstptr, const void* srcptr, uint16_t size);
int memcmp(const void* aptr, const void* bptr, size_t size);
void* memset(void* bufptr, int value, size_t size);
void* memcpy(void* dstptr, const void* srcptr, size_t size);


#endif /* MEMORY_H_ */
