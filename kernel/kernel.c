#include "types.h"
#include "drivers/vga.h"
#include "drivers/io.h"
#include "cpu/idt.h"
#include "string.h"
#include "memory/malloc.h"
#include "std.h"

cursor_t cursor;
heapmap_t heapmap;
ringbuffer_t ringbuffer;


void kernel(void) {
    u32 last_key = ringbuffer.end;
    cursor.x_position = 0;
    cursor.y_position = 3;
    cursor_set_position(cursor.x_position, cursor.y_position);
    
    idt_init();
    pic_remap(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
    pit_init(100);
    heapmap_init(&heapmap);
    ringbuffer_init(&ringbuffer);
    start_interrupts();

    print_centered((u8*)get_cpu_id(), VGA_HEIGHT/2);
    
    while (1) {
        halt();
        stop_interrupts();
        u8 key = read_ringbuffer_end(&ringbuffer);
        start_interrupts();
        if (last_key != ringbuffer.end) {
            handle_character(key);
            last_key = ringbuffer.end;
        }
        print_centered(ringbuffer.buffer, 22);
    }
}
