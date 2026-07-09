#include "types.h"
#include "string.h"
#include "console.h"
#include "ioport.h"
#include "pcb.h"
#include "signal.h"

#include <stdbool.h>

char keyboard_buffer[12312];
int pos = 0;

bool ctrl_pressed = false;

void addCharacter(char c)
{
	keyboard_buffer[pos++] = c;
	keyboard_buffer[pos] = '\0';
    put_cxy(' ', 75, 0);
    put_cxy(' ', 76, 0);
    put_cxy(' ', 77, 0);
    put_cxy(' ', 78, 0);
    put_cxy(c, 79, 0);
}

void keyboard_handler()
{
	U8 scancode = insb(0x60);

	if (scancode == 0x1E) {printk("a"); addCharacter('a');}
    if (scancode == 0x30) {printk("b"); addCharacter('b');}
    if (scancode == 0x2E) {
        if (!ctrl_pressed)
        {
            printk("c"); addCharacter('c');
        
        }
        else
        {
            printk("^C");
            sendsig(current, SIGINT);
        }
    }
    if (scancode == 0x20) {printk("d"); addCharacter('d');}
    if (scancode == 0x12) {printk("e"); addCharacter('e');}
    if (scancode == 0x21) {printk("f"); addCharacter('f');}
    if (scancode == 0x22) {printk("g"); addCharacter('g');}
    if (scancode == 0x23) {printk("h"); addCharacter('h');}
    if (scancode == 0x17) {printk("i"); addCharacter('i');}
    if (scancode == 0x24) {printk("j"); addCharacter('j');}
    if (scancode == 0x25) {printk("k"); addCharacter('k');}
    if (scancode == 0x26) {printk("l"); addCharacter('l');}
    if (scancode == 0x32) {printk("m"); addCharacter('m');}
    if (scancode == 0x31) {printk("n"); addCharacter('n');}
    if (scancode == 0x18) {printk("o"); addCharacter('o');}
    if (scancode == 0x19) {printk("p"); addCharacter('p');}
    if (scancode == 0x10) {printk("q"); addCharacter('q');}
    if (scancode == 0x13) {printk("r"); addCharacter('r');}
    if (scancode == 0x1F) {printk("s"); addCharacter('s');}
    if (scancode == 0x14) {printk("t"); addCharacter('t');}
    if (scancode == 0x16) {printk("u"); addCharacter('u');}
    if (scancode == 0x2F) {printk("v"); addCharacter('v');}
    if (scancode == 0x11) {printk("w"); addCharacter('w');}
    if (scancode == 0x2D) {printk("x"); addCharacter('x');}
    if (scancode == 0x15) {printk("y"); addCharacter('y');}
    if (scancode == 0x2C) {printk("z"); addCharacter('z');}

    if (scancode == 0x39) {
        printk(" "); addCharacter(' ');
        put_cxy('S', 75, 0);
        put_cxy('P', 76, 0);
        put_cxy('A', 77, 0);
        put_cxy('C', 78, 0);
        put_cxy('E', 79, 0);
    }

    if (scancode == 0x1A) {printk("["); addCharacter('[');}
    if (scancode == 0x1B) {printk("]"); addCharacter(']');}
    if (scancode == 0x0D) {printk("="); addCharacter('=');}
    if (scancode == 0x28) {printk("'"); addCharacter('"');}
    if (scancode == 0x29) {printk("`"); addCharacter('`');}
    if (scancode == 0x0C) {printk("-"); addCharacter('-');}
    if (scancode == 0x34) {printk("."); addCharacter('.');}
    if (scancode == 0x33) {printk(","); addCharacter(',');}
    if (scancode == 0x35) {printk("/"); addCharacter('/');}
    if (scancode == 0x27) {printk(";"); addCharacter(';');}
    if (scancode == 0x0F) {printk("     "); addCharacter(' ');}

    if (scancode == 0x1D)
    {
        ctrl_pressed = true;
    }

    if (scancode == 0x9D)
    {
        ctrl_pressed = false;
    }

    if (scancode == 0x01) {sendsig(current, SIGTERM);}

    if (scancode == 0x1C)
    {
        put_cxy('E', 75, 0);
        put_cxy('N', 76, 0);
        put_cxy('T', 77, 0);
        put_cxy('E', 78, 0);
        put_cxy('R', 79, 0);
        if (strcmp(keyboard_buffer, "clear") == 0)
        {
            clear();

        } else if (strcmp(keyboard_buffer, "ps") == 0)
        {
        	ps();
        }

        else if (strcmp(keyboard_buffer, "exit") == 0)
        {
            outw(0xB004, 0x2000); /* Bochs and older versions of QEMU 2.0 */
            outw(0x604, 0x2000); /* Newer versions of qemu */
            outw(0x4004, 0x3400); /* VirtualBox */
            outw(0x600, 0x34); /* Cloud Hypervisor */
        }

        else if (strcmp(keyboard_buffer, "help") == 0)
        {
            printk("\n");
            printk("clear  ps  help  exit");
            printk("\n");
        }

        else if (strcmp(keyboard_buffer, "") == 0)
        {
            printk("\n");

        } else
        {
            printk("\n");
            printk(keyboard_buffer);
            printk(": not found\n");
        }

        printk("# ");

        pos = 0;
        keyboard_buffer[pos] = '\0';
    }

}
