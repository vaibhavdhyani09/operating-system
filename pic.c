#include "io.h"
#include "pic.h"

#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0
#define PIC1_DATA   0x21
#define PIC2_DATA   0xA1

#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT   (PIC2_START_INTERRUPT + 7)
#define PIC_ACK 0x20

/* Remap PIC IRQs 0-15 to interrupts 0x20-0x2F (book points to SigOPS for this) */
void pic_remap(void)
{
    outb(PIC1_PORT_A, 0x11);
    outb(PIC2_PORT_A, 0x11);
    outb(PIC1_DATA, PIC1_START_INTERRUPT);
    outb(PIC2_DATA, PIC2_START_INTERRUPT);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA, 0xFD);   /* unmask only IRQ1 (keyboard) */
    outb(PIC2_DATA, 0xFF);
}

void pic_acknowledge(unsigned int interrupt)
{
    if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT) {
        return;
    }
    if (interrupt < PIC2_START_INTERRUPT) {
        outb(PIC1_PORT_A, PIC_ACK);
    } else {
        outb(PIC2_PORT_A, PIC_ACK);
    }
}
