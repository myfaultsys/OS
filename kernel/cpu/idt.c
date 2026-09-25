#include "types.h"

u64 IDT[256];

u16 IDT_size = sizeof(IDT) - 1;
u64 *IDT_offset = &IDT;

void write_to_IDT(u64 **IDT, u16 interrupt_vector, u64 value) {
    *(IDT_offset + 8*interrupt_vector) = value;
    return;
}
