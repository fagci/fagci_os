struct gdt_entry {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed));

struct gdt_ptr {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

struct gdt_entry gdt[5];
struct gdt_ptr   gp;


void gdtFlush(){
asm volatile("lgdt (%0)" :: "m" (gp));
}

void gdtSetGate(
		int num,
		unsigned long base,
		unsigned long limit,
		unsigned char access,
		unsigned char gran
		) {
	gdt[num].base_low =		(base & 0xFFFF);
	gdt[num].base_middle =	(base >> 16) & 0xFF;
	gdt[num].base_high =	(base >> 24) & 0xFF;
	gdt[num].limit_low =	(limit & 0xFFFF);
	gdt[num].granularity =	(limit >> 16) & 0X0F;
	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}


void initGDT()
{
	gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
	gp.base  = (unsigned int)&gdt;

	gdtSetGate(0, 0, 0, 0, 0);                // Null segment
	gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	gdtSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	gdtSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
	gdtFlush();
}
