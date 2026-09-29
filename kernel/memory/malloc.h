#ifndef H_MALLOC
#define H_MALLOC

#include "types.h"
#include "std.h"
#include "memory/malloc.h"

#define PAGESIZE    (u32)4096

typedef struct page {
    i32 used;
    u8 flags;
    struct page *next;
    u8 buffer[PAGESIZE];
} page_t;

typedef struct {
    void *start;
    void *block_start;
    void *head;
} heap_t;

u8 memcpy(void *source, void *destination, u32 size);
void heap_init(heap_t *heap);
void *malloc(u32 bytes);

#endif