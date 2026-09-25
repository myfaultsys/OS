#include "types.h"
#include "drivers/vga.h"
#include "drivers/io.h"
#include "cpu/idt.h"

volatile u8* vgabuffer = (volatile u8 *)0xb8000;

void kernel(void) {
    //clear_screen();
    idt_init();
    cursor_set_position(79, 24);
    u32 i = 0;
    while (1) {
        for (i32 j = 0; j < 100000000; j++) {}
        cursor_set_position(30 + i % 10, 12 + i % 5);
        spawn_char_vga(30 + i % 10, 12 + i % 5, 'A', 20 + i % 10);
        i++;
    }
    __asm__ volatile (
        "hlt"
        :
        :
        :
    );
}
