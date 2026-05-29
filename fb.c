#include "io.h"
#include "fb.h"

/* The framebuffer memory address */
#define FB_ADDRESS 0x000B8000

/* The framebuffer has 80 columns and 25 rows */
#define FB_ROWS 25
#define FB_COLS 80

/* I/O ports for the framebuffer cursor */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT    0x3D5

/* Commands for moving cursor */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND  15

/* Colors */
#define FB_BLACK      0
#define FB_BLUE       1
#define FB_GREEN      2
#define FB_CYAN       3
#define FB_RED        4
#define FB_MAGENTA    5
#define FB_BROWN      6
#define FB_LIGHT_GREY 7
#define FB_DARK_GREY  8
#define FB_WHITE      15

/* pointer to framebuffer memory */
static char *fb = (char *) FB_ADDRESS;

/* current cursor position */
static unsigned int fb_pos = 0;

/** fb_write_cell:
 * Writes a character with given foreground and background
 * color to position i in the framebuffer
 */
void fb_write_cell(unsigned int i, char c, 
                   unsigned char fg, unsigned char bg)
{
    fb[i]     = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

/** fb_move_cursor:
 * Moves the cursor to the given position
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

/** fb_write:
 * Writes a buffer of characters to the screen
 * automatically moves cursor and handles newlines
 */
int fb_write(char *buf, unsigned int len)
{
    unsigned int i;
    for(i = 0; i < len; i++) {
        /* handle newline character */
        if(buf[i] == '\n') {
            /* move to next row */
            fb_pos = (fb_pos / FB_COLS + 1) * FB_COLS;
        } else {
            /* write character with white text on black background */
            fb_write_cell(fb_pos * 2, buf[i], FB_WHITE, FB_BLUE);
            fb_pos++;
        }
        /* move the cursor to current position */
        fb_move_cursor(fb_pos);
    }
    return 0;
}
