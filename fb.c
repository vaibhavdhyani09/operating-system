#include "fb.h"
#include "io.h"

/* Framebuffer starts at this memory address */
#define FB_ADDRESS      0x000B8000

/* Framebuffer dimensions */
#define FB_ROWS         25
#define FB_COLS         80

/* I/O ports for cursor control */
#define FB_COMMAND_PORT     0x3D4
#define FB_DATA_PORT        0x3D5
#define FB_HIGH_BYTE_CMD    14
#define FB_LOW_BYTE_CMD     15

/* Pointer to framebuffer memory */
static char *fb = (char *) FB_ADDRESS;

/* Current cursor position */
static unsigned int fb_pos = 0;

void fb_write_cell(unsigned int i,
                   char c,
                   unsigned char fg,
                   unsigned char bg)
{
    fb[i * 2]     = c;
    fb[i * 2 + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_CMD);
    outb(FB_DATA_PORT,    (pos >> 8) & 0x00FF);
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_CMD);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

void fb_clear(void)
{
    unsigned int i;
    for (i = 0; i < FB_ROWS * FB_COLS; i++) {
        fb_write_cell(i, ' ', FB_BLACK, FB_BLACK);
    }
    fb_pos = 0;
    fb_move_cursor(0);
}

void fb_write(char *buf, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        if (buf[i] == '\n') {
            /* move to next row */
            fb_pos = fb_pos + (FB_COLS - (fb_pos % FB_COLS));
        } else {
            fb_write_cell(fb_pos, buf[i], FB_WHITE, FB_BLACK);
            fb_pos++;
        }

        /* if we reach the end of screen, go back to top
         * note: proper scrolling is more complex,
         * this is a simple wrap around */
        if (fb_pos >= FB_ROWS * FB_COLS) {
            fb_pos = 0;
        }

        fb_move_cursor(fb_pos);
    }
}
