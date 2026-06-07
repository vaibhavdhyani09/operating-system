# ============================================================
#  Makefile for OS_from_Temu
#  Kernel objects + read-only file system (Unit 12)
# ============================================================

# --- toolchain ---
CC      = gcc
CFLAGS  = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
          -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LD      = ld
LDFLAGS = -T link.ld -melf_i386
AS      = nasm
ASFLAGS = -f elf32

# --- all kernel objects that get linked into kernel.elf ---
OBJECTS = loader.o kmain.o \
          fb.o gdt.o gdt_asm.o idt.o load_idt.o \
          interrupt_handlers.o interrupt_handler.o \
          io.o pic.o keyboard.o pmm.o paging.o \
          tss.o tss_load.o enter_usermode.o \
          fs.o vfs.o

# ============================================================
#  Default target
# ============================================================
all: os.iso

# ============================================================
#  Link the kernel
# ============================================================
kernel.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o kernel.elf

# ============================================================
#  Assembly object rules (.s -> .o)
# ============================================================
loader.o: loader.s
	$(AS) $(ASFLAGS) loader.s -o loader.o

gdt_asm.o: gdt_asm.s
	$(AS) $(ASFLAGS) gdt_asm.s -o gdt_asm.o

load_idt.o: load_idt.s
	$(AS) $(ASFLAGS) load_idt.s -o load_idt.o

interrupt_handlers.o: interrupt_handlers.s
	$(AS) $(ASFLAGS) interrupt_handlers.s -o interrupt_handlers.o

io.o: io.s
	$(AS) $(ASFLAGS) io.s -o io.o

tss_load.o: tss_load.s
	$(AS) $(ASFLAGS) tss_load.s -o tss_load.o

enter_usermode.o: enter_usermode.s
	$(AS) $(ASFLAGS) enter_usermode.s -o enter_usermode.o

paging.o: paging.s
	$(AS) $(ASFLAGS) paging.s -o paging.o

# ============================================================
#  C object rules (.c -> .o)   (CFLAGS ends in -c: compile only)
# ============================================================
kmain.o: kmain.c
	$(CC) $(CFLAGS) kmain.c -o kmain.o

fb.o: fb.c
	$(CC) $(CFLAGS) fb.c -o fb.o

gdt.o: gdt.c
	$(CC) $(CFLAGS) gdt.c -o gdt.o

idt.o: idt.c
	$(CC) $(CFLAGS) idt.c -o idt.o

interrupt_handler.o: interrupt_handler.c
	$(CC) $(CFLAGS) interrupt_handler.c -o interrupt_handler.o

pic.o: pic.c
	$(CC) $(CFLAGS) pic.c -o pic.o

keyboard.o: keyboard.c
	$(CC) $(CFLAGS) keyboard.c -o keyboard.o

pmm.o: pmm.c
	$(CC) $(CFLAGS) pmm.c -o pmm.o

tss.o: tss.c
	$(CC) $(CFLAGS) tss.c -o tss.o

fs.o: fs.c
	$(CC) $(CFLAGS) fs.c -o fs.o

vfs.o: vfs.c
	$(CC) $(CFLAGS) vfs.c -o vfs.o

# ============================================================
#  Host-side file system build tool (NOT kernel code)
# ============================================================
mkfs: mkfs.c
	gcc -Wall -Wextra -o mkfs mkfs.c

# ============================================================
#  Build the read-only file system image
#  Add more files after hello.txt to include them
# ============================================================
fs.img: mkfs hello.txt
	./mkfs fs.img hello.txt

# ============================================================
#  Assemble the bootable ISO (BIOS-only, no EFI hybrid)
# ============================================================
os.iso: kernel.elf fs.img grub.cfg
	mkdir -p iso/boot/grub
	mkdir -p iso/modules
	cp kernel.elf iso/boot/kernel.elf
	cp fs.img     iso/modules/fs.img
	cp grub.cfg   iso/boot/grub/grub.cfg
	grub-mkrescue -o os.iso iso -- -as mkisofs -no-emul-boot

# ============================================================
#  Run in QEMU (boot from CD only, no reboot cycling)
# ============================================================
run: os.iso
	qemu-system-i386 -cdrom os.iso -boot d -no-reboot

# ============================================================
#  Clean
# ============================================================
clean:
	rm -rf *.o kernel.elf mkfs fs.img os.iso \
	       iso/boot/kernel.elf iso/modules/fs.img
