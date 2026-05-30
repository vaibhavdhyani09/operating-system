OBJECTS = loader.o io.o fb.o gdt.o gdt_asm.o kmain.o

CC      = gcc
CFLAGS  = -m32 -nostdlib -nostdinc -fno-builtin \
          -fno-stack-protector -nostartfiles \
          -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS      = nasm
ASFLAGS = -f elf32

all: os.iso

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-A os \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o os.iso \
		iso

run: os.iso
	qemu-system-i386 -cdrom os.iso

loader.o: loader.s
	$(AS) $(ASFLAGS) loader.s -o loader.o

io.o: io.s
	$(AS) $(ASFLAGS) io.s -o io.o

gdt_asm.o: gdt.s
	$(AS) $(ASFLAGS) gdt.s -o gdt_asm.o

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso
