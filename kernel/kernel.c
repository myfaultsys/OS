#include "types.h"
#include "drivers/vga.h"
#include "drivers/io.h"
#include "cpu/idt.h"
#include "string.h"

void kernel(void) {
    idt_init();
    PIC_remap(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
    start_interrupts();

    const u8 text[] = "Printing Works!";

    print(text, 0, 17);

    animation_loop();

    halt();
}
