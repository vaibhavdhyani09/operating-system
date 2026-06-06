#include "gdt.h"

struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char  base_middle;
    unsigned char  access;
    unsigned char  granularity;
    unsigned char  base_high;
} __attribute__((packed));

struct gdt_ptr {
    unsigned short limit;
    unsigned int   base;
} __attribute__((packed));

/* 6 entries: null, kernel code, kernel data, user code, user data, TSS */
static struct gdt_entry gdt[6];
static struct gdt_ptr   gp;

extern void gdt_load(unsigned int);

static void gdt_set_entry(int num,
                           unsigned long base,
                           unsigned long limit,
                           unsigned char access,
                           unsigned char gran)
{
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access      = access;
}

/* called from tss.c after TSS is set up */
void gdt_set_tss(unsigned int base, unsigned int limit)
{
    gdt_set_entry(5, base, limit, 0x89, 0x00);
    /* reload GDT so new entry is visible */
    gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
    gp.base  = (unsigned int) &gdt;
    gdt_load((unsigned int) &gp);
}

void gdt_init(void)
{
    gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
    gp.base  = (unsigned int) &gdt;

    gdt_set_entry(0, 0, 0,          0,    0   ); /* null        */
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); /* kernel code */
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); /* kernel data */
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); /* user code   */
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); /* user data   */
    gdt_set_entry(5, 0, 0,          0,    0   ); /* TSS placeholder */

    gdt_load((unsigned int) &gp);
}
