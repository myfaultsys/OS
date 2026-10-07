#include "types.h"
#include "memory/malloc.h"
#include "drivers/vga.h"
#include "drivers/io.h"

extern heapmap_t heapmap;

void memcpy(void *source, void *destination, u32 size) {
    for (u32 i = 0; i < size; i++) {
        *(u8*)(destination + i) = *(u8*)(source + i);
    }
    return;
}

void memset(void *address, u8 byte, u32 count) {
    for (u32 i = 0; i< count; i++) {
        *(u8*)(address + i) = byte;
    }
    return;
}

void *heapmap_init(heapmap_t *heapmap) {
    heapmap->start = heapmap->block_start = heapmap->end = KERNEL_END + 1;
    heapmap->block_start->index = 0;
    heapmap->block_start->previous = nullptr;
    heapmap->block_start->next = nullptr;
    return heapmap->start;
}

void *allocate_single_page(heapmap_t *heapmap) {
    heapmap->end += PAGEBUFFERSIZE;
    heapmap->block_start = heapmap->end - PAGEBUFFERSIZE;
    return heapmap->block_start; 
}

void *free_single_page(heapmap_t *heapmap) {
    heapmap->end -= PAGEBUFFERSIZE;
    return heapmap->end - PAGEBUFFERSIZE;
}

void *allocate_pages(u32 count) {
    void *ptr;
    page_t *heap_start = (&heapmap)->end + 1;
    u32 page_buffer_count = ALIGN(count)/PAGEBUFFERSIZE;
    for (u32 i = 0; i < page_buffer_count ; i++) {
        ptr = allocate_single_page(&heapmap);
    }
    (&heapmap)->block_start = heap_start;
    for (u32 i = 0; i < page_buffer_count + 1; i++) {
        ((&heapmap)->block_start + i)->index = ((&heapmap)->block_start + i - 1)->index + 1;
        ((&heapmap)->block_start + i)->previous = ((&heapmap)->block_start + i -1);
        ((&heapmap)->block_start + i)->next = ((&heapmap)->block_start + i +1);
    }
    return heap_start;
}

void *free_pages(u32 count) {
    void *ptr;
    for (u32 i = 0; i < count; i++) {
        ptr = free_single_page(&heapmap);
    }
    return ptr;
}