#ifndef H_TYPES
#define H_TYPES

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_CONTROLLER_ADDRESS_PORT 0x3d4
#define VGA_CONTROLLER_DATA_PORT 0x3d5
#define CURSOR_POSITION_SELECT_HIGH_BYTE 0x0e
#define CURSOR_POSITION_SELECT_LOW_BYTE 0x0f
#define PIC_MASTER_COMMAND_PORT 0x0020
#define PIC_MASTER_DATA_PORT    0x0021
#define PIC_SLAVE_COMMAND_PORT  0x00a0
#define PIC_SLAVE_DATA_PORT     0x00a1
#define PIC_END_OF_INTERRUPT    0x0020
#define PIC_INITIALIZE          0x0011
#define PIC_USE_i8086_MODE      0x0001
#define IRQ_CASCADE_IDENTITY    2
#define MASTER_AWARE_OF_SLAVE   1 << IRQ_CASCADE_IDENTITY
#define PIC_UNMASK              0
#define PIC_MASK_ALL            0xff
#define PIC_READ_IRR            0x0a
#define PIC_READ_ISR            0x0b

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

#endif
