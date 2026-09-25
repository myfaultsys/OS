#include "types.h"
#include "io.h"

volatile u8* VGA_buffer = (volatile u8*)0xb8000;

void spawn_char_vga(u8 posx, u8 posy, i8 character, u8 color) {
    volatile u8* VGA_buffer = (volatile u8*)0xb8000;
    if (posy > 80 || posy > 25) return;
    u16 offset = (u16)((posx + VGA_WIDTH*posy)*(u8)2);
    VGA_buffer[offset] = character;
    VGA_buffer[offset + 1] = color;
    return;
}

void vga_ctl_set_low_byte(u8 byte) {
    outb(VGA_CONTROLLER_ADDRESS_PORT, CURSOR_POSITION_SELECT_LOW_BYTE);
    outb(VGA_CONTROLLER_DATA_PORT, byte);
    return;
}

void vga_ctl_set_high_byte(u8 byte) {
    outb(VGA_CONTROLLER_ADDRESS_PORT, CURSOR_POSITION_SELECT_HIGH_BYTE);
    outb(VGA_CONTROLLER_DATA_PORT, byte);
    return;
}

void cursor_set_position(u8 x, u8  y) {
    u16 frame_buffer_offset = (x + VGA_WIDTH*y)*2;
    vga_ctl_set_high_byte((frame_buffer_offset >> 8));
    vga_ctl_set_low_byte(frame_buffer_offset & 0x00ff);
    return;
}

void clear_screen(void) {
    for (u32 i = 0; i < VGA_WIDTH; i++) {
        for (u32 j = 0; j < VGA_HEIGHT; j++) {
            spawn_char_vga(i % VGA_WIDTH, j % VGA_HEIGHT, ' ', 0x00);
        }
    }
    return;
}

void drawing(void) {
    for (u32 i = 0; i < VGA_WIDTH; i++) {
        for (u32 j = 0; j < VGA_HEIGHT; j++) {
            spawn_char_vga(i % VGA_WIDTH, j % VGA_HEIGHT, (i*j + '0') % 110, i*j+i+j % 255);
        }
    }
    return;
}
