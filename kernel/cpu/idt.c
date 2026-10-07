#include "types.h"
#include "idt.h"
#include "drivers/io.h"
#include "drivers/vga.h"
#include "memory/malloc.h"
#include "string.h"

extern cursor_t cursor;
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
    for (u16 i = 0; i < sizeof(IDT)/sizeof(IDT[0]); i++) {
        IDT_set_gate(IDT, i, (u32)isr_stub_table[i], GDT_CODE_SEGMENT_OFFSET, GATE_FLAGS);
    }
    __asm__ volatile (
        "lidt %0" : : "m"(idtr) :
    );
    return;
}
extern u8 is_animation_wanted;
void interrupt_handler(interrupt_t interrupt_vector, u32 error_code) {
    switch (interrupt_vector) {
        case IRQ0: {
            ticks++;
            break;
        }
        case IRQ1: {
            KeyboardScancode_t key = inb(PS2_DATA_PORT);
            if (!(key & KEY_RELEASED_BIT)) {
                if (key == KEY_RIGHT || key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_BACK) goto handle_character;
                spawn_char_vga(cursor.x_position++ % VGA_WIDTH, cursor.y_position % VGA_HEIGHT, scancode_to_ascii[key], WHITEONBLACK);
                if (cursor.x_position == VGA_WIDTH) { cursor.x_position = 0; cursor.y_position++; }
                if (cursor.y_position == VGA_HEIGHT) { cursor.y_position = 0; }
                cursor_set_position(cursor.x_position % VGA_WIDTH, cursor.y_position % VGA_HEIGHT);
            }
            
        handle_character:
            switch(key) {
                case KEY_ESCAPE: {
                    reboot();
                    break;
                }
                case KEY_LCTL: {
                    is_animation_wanted = 0;
                    clear_screen();
                    cursor.x_position = cursor.y_position = 0;
                    cursor_set_position(cursor.x_position, cursor.y_position);
                    break;
                }
                case KEY_UP: {
                    if (cursor.y_position == 0) break;
                    cursor_set_position(cursor.x_position, --cursor.y_position);
                    break;
                }
                case KEY_DOWN: {
                    if (cursor.y_position == VGA_HEIGHT - 1) break;
                    cursor_set_position(cursor.x_position, ++cursor.y_position);
                    break;
                }
                case KEY_RIGHT: {
                    if (cursor.x_position == VGA_WIDTH - 1) break;
                    cursor_set_position(++cursor.x_position, cursor.y_position);
                    break;
                } 
                case KEY_LEFT: {
                    if (cursor.x_position == 0) break;
                    cursor_set_position(--cursor.x_position, cursor.y_position);
                    break;
                }
                case KEY_BACK: {
                    if (cursor.x_position == 0) break;
                    spawn_char_vga(cursor.x_position--, cursor.y_position, ' ', BLACK);
                    cursor_set_position(cursor.x_position, cursor.y_position);
                    spawn_char_vga(cursor.x_position, cursor.y_position, ' ', BLACK);
                    break;
                }
                case KEY_APOST: {
                    //heap_dump();
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    PIC_EOI(interrupt_vector);
    return;
}

void PIC_EOI(u8 irq) {
    if (irq >= 8) {
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
    outb(PIC_SLAVE_DATA_PORT, PIC_UNMASK);
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
    }
    value = inb(port) | 1 << irq_line;
    outb(port, value);
    return;
}

void IRQ_clear_mask(u8 irq_line) {
    u16 port;
    u8 value;

    if (irq_line < 8) {
        port = PIC_MASTER_DATA_PORT;
    } else {
        port = PIC_SLAVE_DATA_PORT;
        irq_line -= 8;
    }
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);
    return;
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

void start_interrupts(void) {
    __asm__ volatile ("sti");
    return;
}
