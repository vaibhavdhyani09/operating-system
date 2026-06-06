#include "tss.h"

struct tss_entry {
    unsigned int  prev_tss;
    unsigned int  esp0;      /* kernel stack pointer */
    unsigned int  ss0;       /* kernel stack segment */
    unsigned int  esp1;
    unsigned int  ss1;
    unsigned int  esp2;
    unsigned int  ss2;
    unsigned int  cr3;
    unsigned int  eip;
    unsigned int  eflags;
    unsigned int  eax, ecx, edx, ebx;
    unsigned int  esp, ebp, esi, edi;
    unsigned int  es, cs, ss, ds, fs, gs;
    unsigned int  ldt;
    unsigned short trap;
    unsigned short iomap_base;
} __attribute__((packed));

static struct tss_entry tss;

/* kernel stack used when CPU switches back to ring 0 */
static unsigned int kernel_stack[1024];

void tss_init(unsigned int kernel_esp)
{
    unsigned int base  = (unsigned int) &tss;
    unsigned int limit = base + sizeof(tss);

    /* zero the whole TSS */
    unsigned char *p = (unsigned char *) &tss;
    unsigned int i;
    for (i = 0; i < sizeof(tss); i++)
        p[i] = 0;

    tss.ss0  = 0x10;          /* kernel data segment */
    tss.esp0 = kernel_esp;    /* kernel stack top    */
    tss.iomap_base = sizeof(tss);

    /* install TSS descriptor into GDT slot 5 (offset 0x28)
       done via gdt_set_tss which we expose from gdt.c */
    extern void gdt_set_tss(unsigned int base, unsigned int limit);
    gdt_set_tss(base, limit);
}

unsigned int tss_get_base(void)
{
    return (unsigned int) &tss;
}
