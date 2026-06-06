OBJECTS = loader.o io.o gdt.o gdt_asm.o load_idt.o interrupt_handlers.o \
          idt.o pic.o keyboard.o fb.o interrupt_handler.o \
          paging.o pmm.o tss.o tss_load.o enter_usermode.o kmain.o

CC      = gcc
CFLAGS  = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
          -nostartfiles -nodefaultlibs -fno-pic -fno-pie -Wall -Wextra -c
LDFLAGS = -T link.ld -melf_i386
AS      = nasm
ASFLAGS = -f elf32

all: kernel.elf program

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

program: program.s
	nasm -f bin program.s -o program

run: kernel.elf program
	qemu-system-i386 \
	    -kernel kernel.elf \
	    -initrd program \
	    -append "" \
	    -m 32 \
	    -vga std

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf program
