#include "types.h"
#include "memory/malloc.h"
#include "drivers/vga.h"
#include "drivers/io.h"

#define ALIGN(n) ((n + sizeof(page_t) - 1) & ~(sizeof(page_t) - 1))

heap_t heap;

u8 memcpy(void *source, void *destination, u32 size) {
    for (u32 i = 0; i < size; i++) {
        *(u8*)(destination + i) = *(u8*)(source + i);
    }
    return 0;
}

void heap_init(heap_t *heap) {
    heap->start = KERNEL_END + 1;
    heap->head = heap->block_start = heap->start;
    return;
}

void *malloc(u32 bytes) {
    u8 *heap_ptr = (u8*)heap.head;
    if (bytes < 0) return nullptr;
    u32 aligned_bytes = ALIGN(bytes);
    u32 page_count = aligned_bytes / sizeof(page_t);
    heap.head += page_count * sizeof(page_t);
    return heap_ptr;
}