# DryNavy OS

A minimal x86 operating system kernel built from scratch, following
The Little Book About OS Development by Erik Helin and Adam Renberg.

## What it does

Boots via GRUB multiboot and runs entirely in 32-bit protected mode.
Sets up a Global Descriptor Table (GDT) for memory segmentation, an
Interrupt Descriptor Table (IDT) for interrupt handling, and remaps
the Programmable Interrupt Controller (PIC) so hardware interrupts
land at the correct vectors. Keyboard input is handled via IRQ1 with
a scancode-to-ASCII translation table. All output goes through a
custom framebuffer driver that writes directly to video memory at
0xB8000 with configurable foreground and background colors. Hardware
communication is done entirely through memory-mapped I/O and I/O
ports with no libraries or shortcuts.

![QEMU Booting](imgs/qemu.png)

![Code Running](imgs/run.png)

## Tools Required

- NASM - assembler
- GCC - C compiler
- GNU LD - linker
- genisoimage - ISO image creation
- QEMU - x86 emulator

## Build

```bash
make
```

## Run

```bash
make run
```

## Clean

```bash
make clean
```
