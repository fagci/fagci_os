#include <stddef.h>
#include <stdint.h>
#include "hal.h"

#define SET_IDT(n) idtSetGate(n, (uintptr_t)isr##n, 0x08, IDT_INTERRUPT_PL0)

#define GATE_PRES(x)      ((x) << 15) // Present
#define GATE_SIZE(x)      ((x) << 11) // Size (0 for 16-bit, 1 for 32)
#define GATE_PRIV(x)     (((x) & 0x03) << 13)   // Set privilege level (0 - 3)

#define GATE_TASK        0x0500
#define GATE_INTERRUPT   0x0600
#define GATE_TRAP        0x0700

#define IDT_TASK_PL0        GATE_PRES(1) | GATE_SIZE(1) | GATE_PRIV(0) | GATE_TASK
#define IDT_INTERRUPT_PL0   GATE_PRES(1) | GATE_SIZE(1) | GATE_PRIV(0) | GATE_INTERRUPT
#define IDT_TRAP_PL0        GATE_PRES(1) | GATE_SIZE(1) | GATE_PRIV(0) | GATE_TRAP

struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

//IDT pointer structure
struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

size_t idt[interruptNum];

struct idt_ptr idtp;

extern "C" void idt_load();

#define EXISR(n) extern "C" void isr##n()

EXISR(0);  EXISR(1);  EXISR(2);  EXISR(3);  EXISR(4);  EXISR(5);  EXISR(6);
EXISR(7);  EXISR(8);  EXISR(9);  EXISR(10); EXISR(11); EXISR(12); EXISR(13);
EXISR(14); EXISR(15); EXISR(16); EXISR(17); EXISR(18); EXISR(19); EXISR(20);
EXISR(21); EXISR(22); EXISR(23); EXISR(24); EXISR(25); EXISR(26); EXISR(27);
EXISR(28); EXISR(29); EXISR(30); EXISR(31); EXISR(32); EXISR(33); EXISR(34);
EXISR(35); EXISR(36); EXISR(37); EXISR(38); EXISR(39); EXISR(40); EXISR(41);
EXISR(42); EXISR(43); EXISR(44); EXISR(45); EXISR(46); EXISR(47);

static void		idtSetGate(unsigned char	index,
					    unsigned int	offset,
					    unsigned short	selector,
					    unsigned short	attribs)
{
  /* Reset the IDT entry. */

  idt[index] = 0;

  /* Configure the gate's offset. */
  idt[index] |= ((offset & 0xffff) | (((unsigned long long int) offset & 0xffff0000) << 32));

  /* Configure the gate's selector. */
  idt[index] |= ((unsigned long long int) selector << 16);

  /* Configure the attributes for this gate descriptor. */
  idt[index] |= ((unsigned long long int) attribs << 32);
}

void remapIrq()
{
	outportb(0x20, 0x11); // Start the init sequence for PIC 1...
	outportb(0xA0, 0x11); // ...and PIC 2
	outportb(0x21, 0x20); // Offset PIC 1 by 32, as the CPU uses the first 32 ISRs
	outportb(0xA1, 0x28); // Offset PIC 2 by another 8 (8 pins on the PIC)
	outportb(0x21, 0x04); // Let PIC 1 know where PIC 2 (the slave) is
	outportb(0xA1, 0x02); // Let PIC 2 know its cascade identity
	outportb(0x21, 0x01); // Put PIC 1 into 8086 mode
	outportb(0xA1, 0x01); // Put PIC 2 into 8086 mode
	outportb(0x21, 0x0);  // Restore saved masks (assuming zero)
	outportb(0xA1, 0x0);
}



void initIDT()
{
	remapIrq();
    idtp.limit=(sizeof(struct idt_entry)*256)-1;
    idtp.base=(unsigned int)&idt;

	SET_IDT(0);  SET_IDT(1);  SET_IDT(2);  SET_IDT(3);  SET_IDT(4);
		SET_IDT(5);  SET_IDT(6);  SET_IDT(7);  SET_IDT(8);  SET_IDT(9);
		SET_IDT(10); SET_IDT(11); SET_IDT(12); SET_IDT(13); SET_IDT(14);
		SET_IDT(15); SET_IDT(16); SET_IDT(17); SET_IDT(18); SET_IDT(19);
		SET_IDT(20); SET_IDT(21); SET_IDT(22); SET_IDT(23); SET_IDT(24);
		SET_IDT(25); SET_IDT(26); SET_IDT(27); SET_IDT(28); SET_IDT(29);
		SET_IDT(30); SET_IDT(31); SET_IDT(32); SET_IDT(33); SET_IDT(34);
		SET_IDT(35); SET_IDT(36); SET_IDT(37); SET_IDT(38); SET_IDT(39);
		SET_IDT(40); SET_IDT(41); SET_IDT(42); SET_IDT(43); SET_IDT(44);
		SET_IDT(45); SET_IDT(46); SET_IDT(47);

    asm volatile("lidt (%0)" :: "m" (idtp));
}

void enableInterrupts(){asm ("sti");}

void disableInterrupts(){asm ("cli");}
