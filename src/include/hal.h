#ifndef __HAL_H
#define __HAL_H

#include <stdint.h>

void enable_interrupts();
void disable_interrupts();
int get_interrupt_state();
void idle();
void die();
unsigned char inportb(unsigned short _port);
void outportb(unsigned short _port, unsigned char _data);


// ------------------------

#define STACKSIZE 0x200000
#define MEMSIZE getTotalMemory()

#define interruptNum 256

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#define COM1 0x3f8
#define COM2 0x2f8
#define COM3 0x3e8
#define COM4 0x2e8

#define ASSERT(x) (x ? (void)0 : crash("\"%s\" returned false in file \"%s\" in function \"%s\" at line %i", #x, __FILE__, __func__, __LINE__))

extern int kend;
#define kern_end  (void *)&kend

void gdtSetGate(
		int num,
		unsigned long base,
		unsigned long limit,
		unsigned char access,
		unsigned char gran
);

typedef struct Registers
{
	unsigned int ds, edi, esi, ebp, esp, ebx, edx, ecx, eax,
	int_no, err, eip, cs, eflags, useresp, ss;
} __attribute__((packed)) Registers;

typedef void (*Handler)(Registers);

uint64_t getTotalMemory();

void crash(char * message, ...);

void initGDT();
void gdtFlush();

void initIDT();
void enableInterrupts();
void disableInterrupts();
void registerInterruptHandler(unsigned char, Handler);


#endif