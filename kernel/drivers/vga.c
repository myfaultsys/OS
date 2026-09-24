#include "types.h"

volatile u8* VGA_buffer = (volatile u8*)0xb8000;

void spawn_char_vga(u8 posx, u8 posy, i8 character, u8 color) {
    volatile u8* VGA_buffer = (volatile u8*)0xb8000;
    if (posy > 80 || posy > 25) return;
    u16 offset = (u16)((posx + VGA_WIDTH*posy)*(u8)2);
    VGA_buffer[offset] = character;
    VGA_buffer[offset + 1] = color;
    return;
}
