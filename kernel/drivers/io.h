#ifndef H_IO
#define H_IO

#define PS2_KEYBOARD_CONTROLLER_PORT 0x64

#define PIT_COMMAND_PORT (u16)0x43
#define PIT_CHANNEL0 (u16)0x40
#define PIT_FREQUENCY 1193182
#define SQUARE_WAVE_MODE 0x36

void outb(u16 port, u8 value);
u8 inb(u16 port);
void pit_init(u32 frequency);
void sleep(u32 count);
void reboot(void);
void halt(void);

#endif
