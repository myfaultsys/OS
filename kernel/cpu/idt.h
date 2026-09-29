#ifndef H_IDT
#define H_IDT

#define PS2_DATA_PORT   0x60
#define PS2_STATUS_PORT 0x64

#define GATE_TYPE           0b1110
#define PRIVILEGE_LEVEL     0b00
#define PRESENT_BIT         0b1
#define ZERO_BIT            0b0

#define GATE_FLAGS ((PRESENT_BIT << 7) | (PRIVILEGE_LEVEL << 5) | (ZERO_BIT << 4) | GATE_TYPE)

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

extern volatile u32 ticks;
extern u8 GDT_code_segment_offset;

void IDT_set_gate(InterruptDescriptor_t *IDT, u16 interrupt_vector, u32 offset, u16 segment, u8 flags);
void idt_init(void);
void interrupt_handler(u32 interrupt_vector, u32 error_code);
void PIC_EOI(u8 irq);
void PIC_remap(u8 master_offset, u8 slave_offset);
void PIC_disable(void);
void IRQ_set_mask(u8 irq_line);
void IRQ_clear_mask(u8 irq_line);
u16 PIC_get_IRR(void);
u16 PIC_get_ISR(void);
void start_interrupts(void);

typedef enum {
    IRQ0 = 0x20,
    IRQ1 = 0x21
} interrupt_t;

enum { GDT_CODE_SEGMENT_OFFSET = 0x08 };

static const u8 scancode_to_ascii[128] = {
    0,   27,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0', '\xE2', '`', '\b',
 '\t',  'q',  'w',  'e',  'r',  't',  'z',  'u',  'i',  'o',  'p', '\x81', '+', '\n',
    0,  'a',  's',  'd',  'f',  'g',  'h',  'j',  'k',  'l', '\x94', '\x84', '^',
    0,  '#',  'y',  'x',  'c',  'v',  'b',  'n',  'm',  ',',  '.',  '-',    0,
  '*',    0,  ' ',    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  '-',    0,    0,    0,  '+',    0,    0,    0,
    0,  '<',    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0
};

#endif
