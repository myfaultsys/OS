#include "types.h"
#include "drivers/vga.h"
#include "drivers/io.h"
#include "cpu/idt.h"
#include "string.h"
#include "memory/malloc.h"

cursor_t cursor;

void kernel(void) {

    cursor.x_position = VGA_WIDTH/2 - 1;
    cursor.y_position = VGA_HEIGHT/2 - 1;
    cursor_set_position(cursor.x_position, cursor.y_position);
    
    idt_init();
    PIC_remap(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
    pit_init(100);
    start_interrupts();
    //dump_heap();
    
    const u8 hello[] = "hello";
    print((void*)hello, 39,18);

    u8 buffer[sizeof(hello)];

    memcpy((void*)hello, (void*)buffer, sizeof(hello));
    print(buffer, 32, 18);

    while (1) {
        sleep(10);
        u8 *ptr = (u8*)init_page();
        animation_loop();

    }
}
