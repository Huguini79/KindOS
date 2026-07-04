OBJECT_FILES = build/entry.asm.o build/ioport.asm.o build/gdt.o build/console.o build/traps.o build/main.o build/keyboard.o build/string.o build/printk.o

CC_FLAGS = -I./ -m32 -c -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Wno-pointer-to-int-cast -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -Os -Iinc
AS_FLAGS = -f elf32
LD_FLAGS = -m elf_i386 -T linker.ld -o iso/boot/kernel $(OBJECT_FILES)
CC = gcc $(CC_FLAGS)
AS = nasm $(AS_FLAGS)
LD = ld $(LD_FLAGS)

GRUB-MKRESCUE = grub-mkrescue
QEMU = qemu-system-x86_64

TARGET = KindOS.iso

all:
	clear
	$(AS) kernel/entry.asm -o build/entry.asm.o
	$(AS) kernel/ioport.asm -o build/ioport.asm.o

	$(CC) kernel/gdt.c -o build/gdt.o
	$(CC) kernel/console.c -o build/console.o
	$(CC) kernel/traps.c -o build/traps.o
	$(CC) kernel/main.c -o build/main.o
	$(CC) kernel/keyboard.c -o build/keyboard.o
	$(CC) kernel/libc/string.c -o build/string.o
	$(CC) kernel/printk.c -o build/printk.o
	$(LD)

	$(GRUB-MKRESCUE) -o $(TARGET) iso/

	$(QEMU) $(TARGET)

clean:
	clear
	rm -rf $(TARGET)
	rm -rf build/*.o build/*.asm.o
