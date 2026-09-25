#include "types.h"
#include "drivers/vga.h"
#include "drivers/io.h"

volatile u8* vgabuffer = (volatile u8 *)0xb8000;

void kernel(void) {
    clear_screen();
    cursor_set_position(80, 25);
    __asm__ volatile (
        "hlt"
        :
        :
        :
    );
}
