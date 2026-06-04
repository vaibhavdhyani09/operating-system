#ifndef INCLUDE_IDT_H
#define INCLUDE_IDT_H

struct idt_entry {
    unsigned short offset_low;
    unsigned short segment_selector;
    unsigned char  reserved;
    unsigned char  type_attr;
    unsigned short offset_high;
} __attribute__((packed));

struct idt {
    unsigned short size;
    unsigned int   address;
} __attribute__((packed));

void idt_init(void);

#endif /* INCLUDE_IDT_H */
