#include "types.h"
#include "printk.h"
#include "ioport.h"
#include "keyboard.h"

struct Idt
{
	U16 isr_low;
	U16 selector;
	U8 reserved;
	U8 type_attributes;
	U16 isr_high;

} __attribute__((packed));

struct Idtr
{
	U16 limit;
	U32 base;

} __attribute__((packed));

struct Idtr idtr;

struct Idt idt_table[8192] __attribute__((aligned(8)));

void setIdtDescriptor(
	U32 vector, U8 type_attributes, 
	void* isr)
{
	struct Idt* Idt = &idt_table[vector];
	Idt->isr_low = (U32)isr & 0xFFFF;
	Idt->selector = 0x08;
	Idt->reserved = 0;
	Idt->type_attributes = type_attributes;
	Idt->isr_high = (U32)isr >> 16 & 0xFFFF;
}

void panic(const char* str)
{
	printk(str);

	while (1) {}
}

volatile U32 ticks = 0;

void zero_exception_handler() {printk("Divide by zero error\n");}
void double_fault_handler() {panic("** KERNEL PANIC: Double Fault #DF **");}
void invalid_tss() {panic("** KERNEL PANIC: Invalid TSS **");}
void clock_handler() {ticks += 1; outb(0x20, 0x20);}
void keyboard_handler_ext() {
	keyboard_handler();
	outb(0x20, 0x20);
}
void syscall_handler() {printk("SYSCALL CALLED\n");}

void TrapsInstall()
{
	setIdtDescriptor(0, 0x8E, zero_exception_handler);
	setIdtDescriptor(0x08, 0x8E, double_fault_handler);
	setIdtDescriptor(0x10, 0x8E, invalid_tss);
	setIdtDescriptor(0x20, 0x8E, clock_handler);
	setIdtDescriptor(0x21, 0x8E, keyboard_handler_ext);
	setIdtDescriptor(0x80, 0x8F, syscall_handler);

	idtr.limit = sizeof(idt_table) - 1;
	idtr.base = (U32)idt_table;

	__asm__ volatile ("lidt %0" :: "m"(idtr));
}
