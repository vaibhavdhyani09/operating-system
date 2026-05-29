#ifndef INCLUDE_FB_H
#define INCLUDE_FB_H

/** fb_write_cell:
 * Writes a character with the given foreground and background 
 * to position i in the framebuffer.
 *
 * @param i   The location in the framebuffer
 * @param c   The character
 * @param fg  The foreground color
 * @param bg  The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

/** fb_move_cursor:
 * Moves the cursor of the framebuffer to the given position
 *
 * @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos);

/** fb_write:
 * Writes a buffer of characters to the screen
 *
 * @param buf The buffer to write
 * @param len The length of the buffer
 */
int fb_write(char *buf, unsigned int len);

#endif /* INCLUDE_FB_H */
