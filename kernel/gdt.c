#include "gdt.h"
#include "pcb.h"

#include "types.h"

/* We define the structure of a 64 bit (8 bytes) Gdt descriptor */
struct Gdt
{
	U16 limit;
	U16 base_low;
	U8 base_mid;
	U8 access_byte;
	U8 flags;
	U8 base_high;

} __attribute__((packed));

/* We define the Gdt Register (6 bytes -> 48 bits) */
struct Gdtr
{
	U16 limit;
	U32 base;

} __attribute__((packed));

struct Gdtr gdtr;

struct Gdt gdt_table[8192] __attribute__((aligned(8))); /* GDT Table Aligned 8 bytes -> 64 bits */

void setTSSDescriptor(struct pcb* pcb)
{
	setGdtDescriptor(pcb->pid+4, sizeof(pcb->tss) - 1, (U32)&pcb->tss, 0x89, 0x0);
}

void setGdtDescriptor(
	U32 n,
	U16 limit,
	U32 base,
	U8 access_byte,
	U8 flags
)
{
	struct Gdt* Gdt = &gdt_table[n]; /* A pointer to that entry in Gdt Table */
	Gdt->limit = limit;
	Gdt->base_low = base & 0xFFFF;
	Gdt->base_mid = base >> 16 & 0xFF;
	Gdt->access_byte = access_byte;
	Gdt->flags = flags;
	Gdt->base_high = base >> 24 & 0xFF;
}

void GdtInstall()
{
	/*
		* Initialize Descriptors
		* First the null segment descriptor
		* Then the code segment descriptor
		* Then the data segment descriptor
		* And finally the tss segment descriptor
	*/
	setGdtDescriptor(0, 0, 0, 0, 0); /* NULL */
	setGdtDescriptor(1, 0xFFFF, 0, 0x9B, 0xCF); /* CODE */
	setGdtDescriptor(2, 0xFFFF, 0, 0x93, 0xCF); /* DATA */

	struct tss tss;
	tss.esp0 = 0x600000;
	tss.ss0 = 0x10;
	tss.eflags = 0x202;
	tss.iopb = 0x8000000;
	setGdtDescriptor(3, sizeof(tss) - 1, (U32)&tss, 0x89, 0x0); /* TSS */

	/*
		* Set up the Gdt Register
	*/
	gdtr.limit = sizeof(gdt_table) - 1; /* LIMIT */
	gdtr.base = (U32)gdt_table; /* BASE */

	/*
		* Finally, load the Gdt register into the CPU
	*/
	__asm__ volatile ("lgdt %0" :: "m"(gdtr));
	__asm__ volatile (
		"mov $0x18, %ax\n\t"
		"ltr %ax"
	);
}
