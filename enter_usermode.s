global enter_usermode

enter_usermode:
    mov eax, [esp + 4]   ; user eip
    mov ecx, [esp + 8]   ; user esp

    ; verify GDT is correct by checking we can load 0x23 into ds
    mov dx, 0x23
    mov ds, dx
    mov es, dx
    mov fs, dx
    mov gs, dx

    ; restore kernel ds before iret setup (safer)
    mov dx, 0x10
    mov ds, dx
    mov es, dx
    mov fs, dx
    mov gs, dx

    push dword 0x23      ; ss
    push ecx             ; user esp
    pushf
    pop  edx
    or   edx, 0x200
    push edx             ; eflags
    push dword 0x1B      ; cs
    push eax             ; eip

    iret

section .note.GNU-stack noalloc noexec nowrite progbits
