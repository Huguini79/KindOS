#include "kernel.h"
#include "console.h"
#include "printk.h"
#include "gdt.h"
#include "pcb.h"
#include "traps.h"
#include "alarm.h"
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

void proc3()
{
	alarm(5);
	printk("\nPROC3\n");
	while (1) {__asm__ volatile ("sti");}
}

void proc2()
{
	alarm(3);
	printk("\nPROC2\n");
	while (1) {__asm__ volatile ("sti");}
}

void proc1()
{
	alarm(6);
	printk("\nPROC1\n");
	while (1) {__asm__ volatile ("sti");}
}

void kernel_main()
{
	remapPIC();
	GdtInstall();
	TrapsInstall();
	ConsoleInstall();
	printk("Hello World!\n");
	printk("In KindOS, we are very kind with everyone :)\n\n# ");

	process1 = createProcess(1, proc1);
	process2 = createProcess(2, proc2);
	struct pcb* process3 = createProcess(3, proc3);
//	int n = exec(process1);

	// yield();

	while (1)
	{
		__asm__ volatile ("sti");
	}

}
