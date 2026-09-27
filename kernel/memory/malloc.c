#include "types.h"
#include "memory/malloc.h"

static mem_t PageTable[100*sizeof(mem_t)];

u8 memcpy(void *source, void *destination, u32 size) {
    for (u32 i = 0; i < size; i++) {
        *(u8*)(destination + i) = *(u8*)(source + i);
    }
    return 0;
}

mem_t *init_page(void) {

    u8 *heap = (u8*)KERNEL_END;

    mem_t page;
    page.index = 0;
    page.next = nullptr;
    //page.page = { 0 };

    u8 *page_address = (u8*)&page;

    memcpy((void*)page_address, (void*)heap, sizeof(mem_t));

    return (mem_t *)page_address;
}

void add_page(u32 count) {

}