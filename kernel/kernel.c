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

    dump_heap();
    sleep(3000);
    while (1) {

        sleep(30);

        const u8 hello[] = "hello";
        print(hello, 0,18);

        u8 buffer[sizeof(hello)];

        memcpy(hello, buffer, sizeof(hello));
        print(buffer, 0, 17);

        u8 *ptr = (u8*)init_page();
    
    animation_loop();

    }
    halt();
}
