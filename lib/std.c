#include "types.h"
#include "std.h"
#include "drivers/io.h"

extern cursor_t cursor;

void ringbuffer_init(ringbuffer_t *buffer) {
    buffer->start = buffer->end = 0;
    return;
}

void rotate_array_left(ringbuffer_t *ringbuffer, u32 steps) {
    for (u32 step = 0; step < steps; step++) {
        u8 temp = ringbuffer->buffer[ringbuffer->end + step];
        for (u32 i = 0; i < ringbuffer->end - ringbuffer->start; i++) {
            ringbuffer->buffer[ringbuffer->end - i - step - 1] = temp;
            temp = ringbuffer->buffer[ringbuffer->end - i - step - 2];
        }
    }
}

void ringbuffer_push(ringbuffer_t *ringbuffer, u8 character) {
    u8 next = (ringbuffer->end + 1) % RINGBUFFERSIZE;
    if (next != ringbuffer->start) {
        ringbuffer->buffer[ringbuffer->end] = character;
        ringbuffer->end = next;
    } else {
        ringbuffer->start += 1;
        ringbuffer->end = ringbuffer->start;
    }
    return;
}

u8 read_ringbuffer_end(ringbuffer_t *ringbuffer) {
    u8 character = ringbuffer->buffer[ringbuffer->end - 1];
    return character;
}

void handle_character(u8 key) {
    if (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT || key == KEY_ESCAPE || key == KEY_LCTL || key == KEY_APOST) {
        switch(key) {
            case KEY_ESCAPE: {
                reboot();
                break;
            }
            case KEY_LCTL: {
                clear_screen();
                cursor.x_position = cursor.y_position = 0;
                cursor_set_position(cursor.x_position, cursor.y_position);
                break;
            }
            case KEY_UP: {
                if (cursor.y_position == 0) break;
                cursor_set_position(cursor.x_position, --cursor.y_position);
                break;
            }
            case KEY_DOWN: {
                if (cursor.y_position == VGA_HEIGHT - 1) break;
                cursor_set_position(cursor.x_position, ++cursor.y_position);
                break;
            }
            case KEY_RIGHT: {
                if (cursor.x_position == VGA_WIDTH - 1) break;
                cursor_set_position(++cursor.x_position, cursor.y_position);
                break;
            } 
            case KEY_LEFT: {
                if (cursor.x_position == 0) break;
                cursor_set_position(--cursor.x_position, cursor.y_position);
                break;
            }
            case KEY_BACK: {
                if (cursor.x_position == 0) break;
                spawn_char_vga(cursor.x_position--, cursor.y_position, ' ', BLACK);
                cursor_set_position(cursor.x_position, cursor.y_position);
                spawn_char_vga(cursor.x_position, cursor.y_position, ' ', BLACK);
                break;
            }
            default:
                break;
        }
    } else {
        draw_keyboard_input(key);
    }
    return;
}