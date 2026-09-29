#include "types.h"
#include "drivers/vga.h"
#include "string.h"

extern cursor_t cursor;

void print(const u8 *text, u8 x, u8 y) {
    u32 offset = 0;
    for (;;) {
        if (*text == '\0') break;
        if (cursor.x_position == VGA_WIDTH - 1 && cursor.y_position == VGA_HEIGHT - 1) { cursor.x_position = cursor.y_position = 0;} 
        if (cursor.x_position == VGA_WIDTH - 1) { cursor.x_position = 0, cursor.y_position++;}
        spawn_char_vga(x+offset++, y, *text++, 0x0f);
    }
    return;
}

void print_centered(const u8 *text, u8 height) {
    u16 length = 0;
    u8 *ptr = text;
    while (*(ptr++) != '\0' && length <= VGA_WIDTH*VGA_HEIGHT) {
        length++;
    }
    if (length > VGA_WIDTH) return;
    u8 x_offset = (VGA_WIDTH - length) / 2;
    print(text, x_offset, height);
    return;
}

void nprint(u8 *text, u32 x, u32 y, u32 count) {
    u32 offset = 0;
    for (u32 i = 0; i < count; i++) {
        //if (*text == '\0') break;
        if (cursor.x_position == VGA_WIDTH - 1 && cursor.y_position == VGA_HEIGHT - 1) { cursor.x_position = cursor.y_position = 0;} 
        if (cursor.x_position == VGA_WIDTH - 1) { cursor.x_position = 0, cursor.y_position++;}
        spawn_char_vga(x+offset++, y, text[i], 0x0f);
    }
    return;
}