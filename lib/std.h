#ifndef H_STD
#define H_STD

#include "drivers/vga.h"

#define RINGBUFFERSIZE (VGA_WIDTH*2)

typedef struct {
    u32 start;
    u32 end;
    u8 buffer[RINGBUFFERSIZE];
} ringbuffer_t;

void ringbuffer_init(ringbuffer_t *ringbuffer);
void rotate_array_left(ringbuffer_t *ringbuffer, u32 steps);
void ringbuffer_push(ringbuffer_t *ringbuffer, u8 character);
u8 read_ringbuffer_end(ringbuffer_t *ringbuffer);
void handle_character(u8 key);

#endif 