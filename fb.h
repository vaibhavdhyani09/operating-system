#ifndef INCLUDE_FB_H
#define INCLUDE_FB_H

#define FB_BLACK 0
#define FB_GREEN 2
#define FB_WHITE 15

void fb_clear(void);
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_write(char *buf, unsigned int len, unsigned char fg, unsigned char bg);
void fb_write_str(const char *str, unsigned char fg, unsigned char bg);

#endif
