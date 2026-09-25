#include "types.h"
#include "idt.h"

extern void isr_wrapper(void);
extern void *isr_stub_table[256];

static InterruptDescriptor_t IDT[256];
static IDTR_t idtr;

static u16 IDT_size = sizeof(IDT) - 1;

void IDT_set_gate(InterruptDescriptor_t *IDT, u16 interrupt_vector, u32 offset, u16 segment, u8 flags) {
    IDT[interrupt_vector].offset_low = offset & 0xffff;
    IDT[interrupt_vector].segment_selector = segment;
    IDT[interrupt_vector].reserved_zero = 0;
    IDT[interrupt_vector].flags = flags;
    IDT[interrupt_vector].offset_high = (offset >> 16) & 0xffff;
    return;
}

void idt_init(void) {
    idtr.base = (u32)&IDT[0];
    idtr.limit = IDT_size;
    for (u8 i = 0; i < 255; i++) {
        IDT_set_gate(IDT, i, (u32)isr_stub_table[i], 0x08, 0b10001110);
    }
    __asm__ volatile (
        "lidt %0"
        :
        : "m"(idtr)
        :
    );
    return;
}

void interrupt_handler(u32 interrupt_vector, u32 error_code) {
    (void)interrupt_vector;
    (void)error_code;
    __asm__ volatile (
        "cli; hlt"
        :
        :
        :
    );
}
