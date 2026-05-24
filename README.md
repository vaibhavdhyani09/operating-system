# Hello Cafebabe OS

A minimal x86 operating system kernel built from scratch, following The Little Book About OS Development by Erik Helin and Adam Renberg.

## What it does

Boots via GRUB and writes 0xCAFEBABE to the EAX register, proving the entire toolchain and boot process works correctly.

![QEMU Booting](imgs/qemu.png)

![Code Running](imgs/run.png)

## Tools Required

- NASM - assembler
- GNU LD - linker
- genisoimage - ISO image creation
- QEMU - x86 emulator

## Build

```bash
nasm -f elf32 loader.s -o loader.o
ld -T link.ld -melf_i386 loader.o -o kernel.elf
cp kernel.elf iso/boot/kernel.elf
genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot \
    -boot-load-size 4 -A os -input-charset utf8 -quiet \
    -boot-info-table -o os.iso iso
```

## Run

```bash
qemu-system-i386 -cdrom os.iso -display gtk
```

## Verify

```bash
qemu-system-i386 -cdrom os.iso -monitor stdio
```
Then type `info registers` and check that EAX=CAFEBABE.

## Reference

The Little Book About OS Development - https://littleosbook.github.io
