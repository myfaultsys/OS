#ifndef H_MALLOC
#define H_MALLOC

#include "types.h"
#include "std.h"
#include "memory/malloc.h"

#define PAGEBUFFERSIZE    (u32)4096
#define ALIGN(n) ((n + PAGEBUFFERSIZE - 1) & ~(PAGEBUFFERSIZE - 1))

typedef struct page {
    i32 used;
    u8 flags;
    u32 index;
    u32 blocks;
    struct page *next;
    struct page *previous;
    u8 buffer[PAGEBUFFERSIZE];
} page_t;

typedef struct {
    page_t *start;
    page_t *block_start;
    page_t *end;
} heapmap_t;

void memcpy(void *source, void *destination, u32 size);
void memset(void *address, u8 byte, u32 count);
void *heapmap_init(heapmap_t *heapmap);
void *allocate_single_page(heapmap_t *heapmap);
void *free_single_page(heapmap_t *heapmap);
void *allocate_pages(u32 count);
void *free_pages(u32 count);

#endif