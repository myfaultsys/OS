#ifndef H_IO
#define H_IO

#define PS2_KEYBOARD_CONTROLLER_PORT 0x64

/* Bit 0 of this port is connected to the CPU's hardware reset line and writing a zero to it instructs the PS/2 controller to pull that line low. Reboot. */

void outb(u16 port, u8 value);
u8 inb(u16 port);

#endif
