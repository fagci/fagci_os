#include "hal.h"


/*static const char *interrupt_names[] =
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
};*/

static Handler handlers[interruptNum];

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

void irqHandler(Registers regs)
{
	if (regs.int_no >= IRQ8) {
		// This interrupt involved the slave, send reset to it
		outportb(0xA0, 0x20);
	}

	// Reset master
	outportb(0x20, 0x20);

	Handler handler = handlers[regs.int_no];
	if (handler){
		handler(regs);
	}
}

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

void isrHandler(Registers regs)
{
	/*terminal_writestring("ISR ");
	char so[33];
	itoa(regs.int_no, so, 16);
	terminal_writestring(so);
	terminal_writestring("\n");*/

	Handler handler = handlers[regs.int_no];
	if (handler)
		handler(regs);
}

void registerInterruptHandler(unsigned char i, Handler handler)
{
	handlers[i] = handler;
}
