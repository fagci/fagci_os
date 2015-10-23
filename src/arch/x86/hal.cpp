#include <stdint.h>

void enable_interrupts() {
  __asm__ volatile("sti");
}

void disable_interrupts() {
  __asm__ volatile("cli");
}

int get_interrupt_state() {
  uint32_t eflags;
  __asm__ volatile("pushf; pop %0" : "=r" (eflags));
  return eflags & 0x200;
}

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
