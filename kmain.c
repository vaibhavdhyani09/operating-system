#include "gdt.h"
#include "fb.h"

void kmain(void)
{
    /* Clear the screen first */
    fb_clear();

    /* Set up segmentation */
    gdt_init();

    /* Confirm it worked with a message */
    fb_write("GDT initialised successfully!", 29);
    fb_write("\n", 1);
    fb_write("Segmentation is set up.", 23);

    /* Loop forever */
    for(;;);
}
