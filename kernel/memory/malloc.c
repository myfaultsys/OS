#include "types.h"
#include "memory/malloc.h"
#include "drivers/vga.h"
#include "drivers/io.h"

static mem_t PageTable[100*sizeof(mem_t)];

u8 memcpy(void *source, void *destination, u32 size) {
    for (u32 i = 0; i < size; i++) {
        *(u8*)(destination + i) = *(u8*)(source + i);
    }
    return 0;
}

mem_t *init_page(void) {

    u8 *heap = (u8*)KERNEL_END;
    u8 buffer[4096];
    for (u32 i = 0; i < 4096; i++) {
        buffer[i] = 1 + '0';
    }

    mem_t page;
    page.index = 18;
    page.next = (void*)5;
    memcpy(buffer, page.page, sizeof(buffer));

    u8 *page_address = (u8*)&page;

    memcpy((void*)page_address, (void*)heap, 4100);

    return (mem_t *)page_address;
}

void dump_heap(void) {
    for (u32 i = 0; i < sizeof(mem_t); i++) {
        sleep(3);
        if (i == 80*25) {
            clear_screen();
        }
        spawn_char_vga(i % 80, (i / 80) % 25, *((u8*)KERNEL_END+i)+'0', 0x0f);
        i++;
    }
    sleep(10);
    clear_screen();
    return;
}