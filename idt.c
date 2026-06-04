#include "idt.h"

#define IDT_ENTRIES 256
#define KERNEL_CODE_SEGMENT 0x08
#define IDT_INTERRUPT_GATE_PL0 0x8E

static struct idt_entry idt_entries[IDT_ENTRIES];

extern void interrupt_handler_0(void);
extern void interrupt_handler_1(void);
extern void interrupt_handler_33(void);   /* remapped keyboard (IRQ1 -> 0x21) */

extern void load_idt(unsigned int idt_ptr_address);

static void idt_set_entry(unsigned char num, unsigned int handler,
                          unsigned short selector, unsigned char type_attr)
{
    idt_entries[num].offset_low       = handler & 0xFFFF;
    idt_entries[num].segment_selector = selector;
    idt_entries[num].reserved         = 0;
    idt_entries[num].type_attr        = type_attr;
    idt_entries[num].offset_high      = (handler >> 16) & 0xFFFF;
}

void idt_init(void)
{
    struct idt idt_ptr;

    idt_set_entry(0,  (unsigned int) interrupt_handler_0,
                  KERNEL_CODE_SEGMENT, IDT_INTERRUPT_GATE_PL0);
    idt_set_entry(1,  (unsigned int) interrupt_handler_1,
                  KERNEL_CODE_SEGMENT, IDT_INTERRUPT_GATE_PL0);
    idt_set_entry(33, (unsigned int) interrupt_handler_33,
                  KERNEL_CODE_SEGMENT, IDT_INTERRUPT_GATE_PL0);

    idt_ptr.size    = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idt_ptr.address = (unsigned int) &idt_entries;

    load_idt((unsigned int) &idt_ptr);
}
