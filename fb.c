#include "fb.h"

#define FB_ADDR 0x000B8000
#define FB_CELLS (80 * 25)

static char *fb = (char *) FB_ADDR;
static unsigned int cursor = 0;

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i]     = c;
    fb[i + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

void fb_clear(void)
{
    unsigned int i;
    for (i = 0; i < FB_CELLS; i++) {
        fb_write_cell(i * 2, ' ', FB_WHITE, FB_BLACK);
    }
    cursor = 0;
}

void fb_write(char *buf, unsigned int len, unsigned char fg, unsigned char bg)
{
    unsigned int j;
    for (j = 0; j < len; j++) {
        fb_write_cell(cursor, buf[j], fg, bg);
        cursor += 2;
    }
}
