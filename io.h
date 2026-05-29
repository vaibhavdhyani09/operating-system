#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/** outb:
 * Sends a byte to the given I/O port
 * @param port  The I/O port number
 * @param data  The byte to send
 */
void outb(unsigned short port, unsigned char data);

/** inb:
 * Reads a byte from the given I/O port
 * @param port  The I/O port number
 * @return      The byte read
 */
unsigned char inb(unsigned short port);

#endif 
