#ifndef H_STD
#define H_STD

#define RINGBUFFERSIZE 32

typedef struct {
    u8 *start;
    u8 *end;
    u8 buffer[RINGBUFFERSIZE];
} ringbuffer_t;

void ringbuffer_init(ringbuffer_t *buffer);

#endif 