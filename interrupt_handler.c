#include "pic.h"
#include "keyboard.h"
#include "fb.h"

static const char scancode_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' '
};

void interrupt_handler(unsigned int interrupt)
{
    if (interrupt == 33) {
        unsigned char code = read_scan_code();
        if (code < 128) {
            char c = scancode_ascii[code];
            if (c) {
                fb_write(&c, 1, FB_BLACK, FB_WHITE);
            }
        }
        pic_acknowledge(interrupt);
    }
}
