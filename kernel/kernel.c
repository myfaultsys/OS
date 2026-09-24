#include "../lib/types.h"
#include "../lib/graphics.h"

void kernel(void) {
    volatile u8* vgabuffer = (volatile u8 *)0xb8000;
    for (u32 i = 0; i < 80*25; i++) {
        //vgabuffer[i++] = (u8)'a';
        //vgabuffer[i] = (u8)0xbc;
        spawn_char_vga(i % 79, i % 24, i + '0', i % 254);
    }
    //spawn_char(10,14,'a',0xf0);
    __asm__ volatile (
        "hlt"
        :
        :
        :
    );
}
