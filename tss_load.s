global tss_load

tss_load:
    mov ax, 0x28    ; TSS is GDT slot 5: 5*8 = 40 = 0x28, RPL=0
    ltr ax
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
