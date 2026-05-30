#ifndef INCLUDE_FB_H
#define INCLUDE_FB_H

/* Available colors for foreground and background */
#define FB_BLACK        0
#define FB_BLUE         1
#define FB_GREEN        2
#define FB_CYAN         3
#define FB_RED          4
#define FB_MAGENTA      5
#define FB_BROWN        6
#define FB_LIGHT_GREY   7
#define FB_DARK_GREY    8
#define FB_LIGHT_BLUE   9
#define FB_LIGHT_GREEN  10
#define FB_LIGHT_CYAN   11
#define FB_LIGHT_RED    12
#define FB_LIGHT_MAGENTA 13
#define FB_LIGHT_BROWN  14
#define FB_WHITE        15

/** fb_write_cell - write a character to the framebuffer
 * @param i  Position in framebuffer
 * @param c  Character to write
 * @param fg Foreground color
 * @param bg Background color
 */
void fb_write_cell(unsigned int i,
                   char c,
                   unsigned char fg,
                   unsigned char bg);

/** fb_move_cursor - move the framebuffer cursor
 * @param pos New cursor position
 */
void fb_move_cursor(unsigned short pos);

/** fb_write - write a string to the screen
 * @param buf The string to write
 * @param len Length of the string
 */
void fb_write(char *buf, unsigned int len);

/** fb_clear - clear the entire screen */
void fb_clear(void);

#endif /* INCLUDE_FB_H */
