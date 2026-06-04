global loader
extern kmain

MAGIC_NUMBER      equ 0x1BADB002
ALIGN_MODULES     equ 0x00000001
CHECKSUM          equ -(MAGIC_NUMBER + ALIGN_MODULES)
KERNEL_STACK_SIZE equ 4096

section .text
    align 4
    dd MAGIC_NUMBER
    dd ALIGN_MODULES
    dd CHECKSUM

loader:
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    push ebx
    call kmain

.loop:
    jmp .loop

section .bss
    align 4
kernel_stack:
    resb KERNEL_STACK_SIZE
