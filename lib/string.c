#include "types.h"
#include "drivers/vga.h"
#include "string.h"

void print(u8 *text, u8 x, u8 y) {
    u32 offset = 0;
    for (;;) {
        if (*text == '\0') break;
        spawn_char_vga(x+offset++, y, *text++, 0x0f);
    }
    return;
}