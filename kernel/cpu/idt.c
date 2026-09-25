#include "types.h"
#include "idt.h"
#include "drivers/io.h"

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
        "lidt %0" : : "m"(idtr) :
    );
    return;
}

void interrupt_handler(u32 interrupt_vector, u32 error_code) {
    (void)interrupt_vector;
    (void)error_code;
    __asm__ volatile (
        "cli; hlt" : : :
    );
}

void PIC_EOI(u8 irq) {
    if (irq > 7) {
        outb(PIC_SLAVE_COMMAND_PORT, PIC_END_OF_INTERRUPT);
    }
    outb(PIC_MASTER_COMMAND_PORT, PIC_END_OF_INTERRUPT);
    return;
}

void PIC_remap(u8 master_offset, u8 slave_offset) {
    outb(PIC_MASTER_COMMAND_PORT, PIC_INITIALIZE);
    outb(PIC_SLAVE_COMMAND_PORT, PIC_INITIALIZE);
    outb(PIC_MASTER_DATA_PORT, master_offset);
    outb(PIC_SLAVE_DATA_PORT, slave_offset);
    outb(PIC_MASTER_DATA_PORT, MASTER_AWARE_OF_SLAVE);
    outb(PIC_SLAVE_DATA_PORT, IRQ_CASCADE_IDENTITY);
    outb(PIC_MASTER_DATA_PORT, PIC_USE_i8086_MODE);
    outb(PIC_SLAVE_DATA_PORT, PIC_USE_i8086_MODE);
    outb(PIC_MASTER_DATA_PORT, PIC_UNMASK);
    outb(PIC_SLAVE_COMMAND_PORT, PIC_UNMASK);
    return;
}

void PIC_disable(void) {
    outb(PIC_MASTER_DATA_PORT, PIC_MASK_ALL);
    outb(PIC_SLAVE_DATA_PORT, PIC_MASK_ALL);
    return;
}

void IRQ_set_mask(u8 irq_line) {
    u16 port;
    u8 value;

    if (irq_line < 8) {
        port = PIC_MASTER_DATA_PORT;
    } else {
        port = PIC_SLAVE_DATA_PORT;
        irq_line -= 8;
        value = inb(port) | 1 << irq_line;
        outb(port, value);
        return;
    }
}

void IRQ_clear_mask(u8 irq_line) {
    u16 port;
    u8 value;

    if (irq_line < 8) {
        port = PIC_MASTER_DATA_PORT;
    } else {
        port = PIC_SLAVE_DATA_PORT;
        irq_line -= 8;
        value = inb(port) & ~(1 << irq_line);
        outb(port, value);
        return;
    }
}

u16 PIC_get_IRR(void) {
    outb(PIC_MASTER_COMMAND_PORT, PIC_READ_IRR);
    outb(PIC_SLAVE_DATA_PORT, PIC_READ_IRR);
    return (inb(PIC_SLAVE_DATA_PORT) << 8 | inb(PIC_MASTER_DATA_PORT));
}

u16 PIC_get_ISR(void) {
    outb(PIC_MASTER_COMMAND_PORT, PIC_READ_ISR);
    outb(PIC_SLAVE_DATA_PORT, PIC_READ_ISR);
    return (inb(PIC_SLAVE_DATA_PORT) << 8 | inb(PIC_MASTER_DATA_PORT));
}
