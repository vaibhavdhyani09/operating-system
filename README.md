# DryNavy OS

A minimal x86 operating system kernel built from scratch, following 
The Little Book About OS Development by Erik Helin and Adam Renberg.

## What it does

Boots via GRUB and displays text on screen using a custom framebuffer 
driver written entirely from scratch. Hardware communication is done 
manually through memory mapped I/O and I/O ports with no libraries or 
shortcuts. Serial port logging is also set up to redirect kernel output 
to a file for debugging.

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


