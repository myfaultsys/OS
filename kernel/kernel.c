#include "types.h"
#include "drivers/vga.h"
#include "drivers/io.h"
#include "cpu/idt.h"

volatile u8* vgabuffer = (volatile u8 *)0xb8000;

void kernel(void) {

    idt_init();
    PIC_remap(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
    start_interrupts();

    animation_loop();

    halt();
}
