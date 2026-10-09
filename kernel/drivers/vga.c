#include "types.h"
#include "io.h"
#include "std.h"

volatile u8* VGA_buffer = (volatile u8*)0xb8000;
extern ringbuffer_t ringbuffer;
extern cursor_t cursor;

void spawn_char_vga(const u8 posx, u8 posy, i8 character, u8 color) {
    volatile u8* VGA_buffer = (volatile u8*)0xb8000;
    if (posy > VGA_WIDTH || posy > VGA_HEIGHT) return;
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
    u16 frame_buffer_offset = (x + VGA_WIDTH*y);
    vga_ctl_set_high_byte((frame_buffer_offset >> 8));
    vga_ctl_set_low_byte(frame_buffer_offset & 0x00ff);
    return;
}

void clear_screen(void) {
    for (u32 i = 0; i < VGA_WIDTH; i++) {
        for (u32 j = 0; j < VGA_HEIGHT; j++) {
            spawn_char_vga(i % VGA_WIDTH, j % VGA_HEIGHT, ' ', 0x0f);
        }
    }
    return;
}

void draw_keyboard_input(u8 key) {
    spawn_char_vga(cursor.x_position++ % VGA_WIDTH, cursor.y_position % VGA_HEIGHT, key, WHITEONBLACK);
    if (cursor.x_position == VGA_WIDTH) { cursor.x_position = 0; cursor.y_position++; }
    if (cursor.y_position == VGA_HEIGHT) { cursor.y_position = 0; }
    cursor_set_position(cursor.x_position % VGA_WIDTH, cursor.y_position % VGA_HEIGHT);
    return;
}