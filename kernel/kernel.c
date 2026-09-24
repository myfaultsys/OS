#include "types.h"
#include "drivers/vga.h"

volatile u8* vgabuffer = (volatile u8 *)0xb8000;

void kernel(void) {
    for (u32 i = 0; i < VGA_WIDTH; i++) {
        for (u32 j = 0; j < VGA_HEIGHT; j++) {
            spawn_char_vga(i % VGA_WIDTH, j % VGA_HEIGHT, (i*j + '0') % 110, i*j+i+j % 255);
        }
    }
    __asm__ volatile (
        "hlt"
        :
        :
        :
    );
}
