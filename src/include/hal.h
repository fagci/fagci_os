#ifndef __HAL_H
#define __HAL_H

void enable_interrupts();
void disable_interrupts();
int get_interrupt_state();
void idle();
void die();
unsigned char inportb(unsigned short _port);
void outportb(unsigned short _port, unsigned char _data);


#endif