#include "types.h"
#include "io.h"

void outb(u16 port, u8 value) {
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
        :
    );
    return;
}

u8 inb(u16 port) {
    u8 port_return_value;
    __asm__ volatile(
        "inb %1, %0"
        : "=a"(port_return_value)
        : "Nd"(port)
        :
    );
    return port_return_value;
}

void reboot(void) {
    outb(PS2_KEYBOARD_CONTROLLER_PORT, 0xfe);
}

void halt(void) {
    __asm__ volatile ("hlt");
}
