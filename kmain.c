#include "fb.h"

int kmain(void)
{
    char *str = "It finally works! @drynavy signing off.";
    

    /* count length of string */
    unsigned int len = 0;
    while(str[len] != '\0') {
        len++;
    }

    /* write to screen */
    fb_write(str, len);

    return 0;
}
