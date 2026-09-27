#include "types.h"
#include "drivers/vga.h"
#include "drivers/io.h"
#include "cpu/idt.h"
#include "string.h"
#include "memory/malloc.h"

void kernel(void) {

    idt_init();
    PIC_remap(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
    pit_init(100);
    start_interrupts();

    while (1) {

        sleep(30);

        u8 text[] = "Printing And Memory (kinda) Work!";
        u8 buffer[sizeof(text)];

        memcpy(text, buffer, sizeof(text));
        print(text, 0, 17);

        u8 *ptr = (u8*)init_page();

        animation_loop();

    }
    halt();
}
