#include "types.h"
#include "drivers/vga.h"
#include "drivers/io.h"
#include "cpu/idt.h"
#include "string.h"
#include "memory/malloc.h"

cursor_t cursor;

extern heap_t heap;
const u8 heaptest[] =   "This String Is Stored In Heap Memory!";
const u8 hello[] =      "hello";

void kernel(void) {

    cursor.x_position = 0;
    cursor.y_position = 3;

    cursor_set_position(cursor.x_position, cursor.y_position);
    
    idt_init();
    PIC_remap(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
    pit_init(100);
    heap_init(&heap);
    start_interrupts();

    u8 *ptr = (u8*)malloc(1);

    memcpy(heaptest, ptr, sizeof(heaptest));

    print((void*)hello, 39,18);
    
    print_centered(ptr, 10);

    sleep(1000);
    
    u8 buffer[sizeof(hello)];

    memcpy((void*)hello, (void*)buffer, sizeof(hello));

    print(buffer, 32, 18);

    while (1) {
        sleep(10);
        animation_loop();
    }
}
