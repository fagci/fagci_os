#include <stdint.h>
#include "hal.h"

void enable_interrupts() {
  __asm__ volatile("sti");
}

void disable_interrupts() {
  __asm__ volatile("cli");
}

//int get_interrupt_state() {
//  uint32_t eflags;
//  __asm__ volatile("pushf; pop %0" : "=r" (eflags));
//  return eflags & 0x200;
//}

void idle() {
  for (;;) {
    __asm__ volatile("hlt");
  }
}

void die() {
  disable_interrupts();
  idle();
}

unsigned char inportb(unsigned short _port) {
  unsigned char rv;
  __asm__ volatile("inb %1, %0" : "=a" (rv) : "dN" (_port));
  return rv;
}

void outportb(unsigned short _port, unsigned char _data) {
  __asm__ volatile("outb %1, %0" : : "dN" (_port), "a" (_data));
}



static const char *interrupt_names[] =
{
	"Division by zero",
	"Debug exception",
	"Non-maskable interrupt",
	"Breakpoint exception",
	"Overflow",
	"Out of bounds exception",
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	"Bad TSS",
	"Segment not present",
	"Stack fault",
	"General Protection Fault",
	"Page fault",
	"Unknown interrupt exception",
	"Coprocessor fault",
	"Alignment check exception",
	"Machine check exception"
};

static Handler handlers[NUM_IDT_ENTRIES];

extern "C" {
    void irq_handler(Registers regs)
    {
        if (regs.int_no >= IRQ8) {
            // This interrupt involved the slave, send reset to it
            outb(0xA0, 0x20);
        }

        // Reset master
        outb(0x20, 0x20);

        Handler handler = handlers[regs.int_no];
        if (handler)
            handler(regs);
    }

    void isr_handler(Registers regs)
    {
        Handler handler = handlers[regs.int_no];
        if (handler)
            handler(regs);
        else
            PANIC(interrupt_names[regs.int_no]);
    }
}

void register_interrupt_handler(uint8_t i, Handler handler)
{
	handlers[i] = handler;
}
