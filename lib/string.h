#ifndef H_STRING
#define H_STRING

#include "types.h"

typedef struct {
    u8 bits32to25;
    u8 bits24to17;
    u8 bits16to9;
    u8 bits8to0;
    u8 nullbyte;
}__attribute__((packed)) u32_split_t;

void print(const u8 *text, u8 x, u8 y);
void print_centered(const u8 *text, u8 height);
void nprint(u8 *text, u32 x, u32 y, u32 count);

#endif
