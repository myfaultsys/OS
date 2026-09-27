#ifndef H_MALLOC
#define H_MALLOC

#include "types.h"

#define PAGE_SIZE 4096

typedef struct memory_page {
    u32 index;
    struct memory_page *next;
    u8 page[PAGE_SIZE];
}__attribute__((packed)) mem_t;

u8 memcpy(void *source, void *destination, u32 size);
mem_t *init_page(void);

#endif