/*
 * serial.h
 *
 *  Created on: Oct 21, 2015
 *      Author: fagci
 */

#include <stdint.h>
#include <stddef.h>

#ifndef SERIAL_H_
#define SERIAL_H_

#define COM1 0x3f8

void init_serial(size_t port);
int is_transmit_empty(uint16_t port);
void write_serial(uint16_t port, char a);
void print_serial(uint16_t port, const char* s);

#endif /* SERIAL_H_ */
