#ifndef H_IDT
#define H_IDT

typedef struct {
    u16 offset_low;
    u16 segment_selector;
    u8 reserved_zero;
    u8 flags;
    u16 offset_high;
}__attribute__((packed)) InterruptDescriptor_t;

typedef struct {
    u16 limit;
    u64 base;
}__attribute__((packed)) IDTR_t;

void IDT_set_gate(InterruptDescriptor_t *IDT, u16 interrupt_vector, u32 offset, u16 segment, u8 flags);
void idt_init(void);
void interrupt_handler(void);

#endif
