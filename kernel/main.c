#include "kernel.h"
#include "console.h"
#include "printk.h"
#include "gdt.h"
#include "pcb.h"
#include "traps.h"
#include "ioport.h"

void remapPIC()
{
	/* ICW 1 */
	U8 icw1_all = 0x11;
	outb(0x20, icw1_all);
	outb(0xA0, icw1_all);

	/* ICW 2 */
	U8 icw2_map_21h = 0x20;
	U8 icw2_map_A1h = 0x28;
	outb(0x21, icw2_map_21h);
	outb(0xA1, icw2_map_A1h);

	/* ICW 3 */
	U8 icw3_ir_line_21h = 0x4;
	U8 icw3_ir_line_A1h = 0x2;
	outb(0x21, icw3_ir_line_21h);
	outb(0xA1, icw3_ir_line_A1h);

	/* ICW 4 */
	U8 icw4_80x86_mode_all = 1;
	outb(0x21, icw4_80x86_mode_all);
	outb(0xA1, icw4_80x86_mode_all);
}

struct pcb* process1;
struct pcb* process2;

void proc2()
{
	printk("Process 2: Hello World!\n");
	yield();
	printk("Process 2: I'm back!\n");
	yield();
}

void proc1()
{
	printk("Process 1: Hello World!\n");
	yield();
	printk("Process 1: I'm back!\n");
	yield();
}

void kernel_main()
{
	remapPIC();
	GdtInstall();
	TrapsInstall();
	ConsoleInstall();
	printk("Hello World!\n");
	printk("In KindOS, we are very kind with everyone :)\n\n# ");

	process1 = createProcess(0, proc1);
	process2 = createProcess(1, proc2);
//	int n = exec(process1);

	yield();

	while (1)
	{
		__asm__ volatile ("sti");
	}

}
