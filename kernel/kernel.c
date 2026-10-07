#include "types.h"
#include "drivers/vga.h"
#include "drivers/io.h"
#include "cpu/idt.h"
#include "string.h"
#include "memory/malloc.h"

cursor_t cursor;
heapmap_t heapmap;

const u8 heaptest[] =       "This String Is Stored In Heap Memory!";
const u8 heaptest2[] =      "This String Is Stored In Heap Memory Further Away!";
const u8 hello[] =          "hello";

u8 is_animation_wanted = 1;
u8 overflow[15097];

u8 ringbuffer[32];

void kernel(void) {
    cursor.x_position = 0;
    cursor.y_position = 3;
    cursor_set_position(cursor.x_position, cursor.y_position);
    
    idt_init();
    PIC_remap(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
    pit_init(100);
    heapmap_init(&heapmap);
    start_interrupts();

    u8 *pHeap = allocate_single_page(&heapmap);
    memcpy(heaptest, pHeap, sizeof(heaptest));

    page_t *pHeap2 = allocate_pages(3);
    memcpy(heaptest2, pHeap2->buffer, sizeof(heaptest2));
    
    u8 size = 54;
    u8 buff[size];
    memcpy(&(pHeap2->index), buff, 4);
    for (u8 i = 0; i < sizeof(buff)/sizeof(buff[0]); i++) {
        buff[i] += '0';
    }
    memset(&buff[size], '\0', 1);

    print_centered(pHeap, 6);
    print_centered(pHeap2->buffer, 8);
    print_centered((u8*)(pHeap2->index), 10);
    print_centered(buff, 11);

    while (1) {
        sleep(10);
        if (is_animation_wanted) {
            animation_loop();
        }
    }
}
