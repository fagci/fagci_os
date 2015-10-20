/*
 * ports.h
 *
 *  Created on: Oct 21, 2015
 *      Author: fagci
 */
#include <stdint.h>

#ifndef PORTS_H_
#define PORTS_H_


static inline void outb (uint16_t p_port,uint8_t p_data) {
    asm volatile ("outb %1, %0" : : "dN" (p_port), "a" (p_data));
}

static inline uint8_t inb( uint16_t p_port)
{
    uint8_t l_ret;
    asm volatile("inb %1, %0" : "=a" (l_ret) : "dN" (p_port));
    return l_ret;
}

#endif /* PORTS_H_ */
