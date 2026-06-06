bits 32

global _start
_start:
    mov eax, 0xCAFEBABE
.loop:
    jmp .loop
