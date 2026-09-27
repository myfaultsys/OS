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
#define PIC_MASTER_OFFSET       0x20
#define PIC_SLAVE_OFFSET        0x28

#define PS2_CONTROLLER          0x21

#define KEY_RELEASED_BIT        0b10000000

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

extern void *KERNEL_END;

typedef enum {
    KEY_ESCAPE  = 0x01,
    KEY_1       = 0x02,
    KEY_2       = 0x03,
    KEY_3       = 0x04,
    KEY_4       = 0x05,
    KEY_6       = 0x07,
    KEY_7       = 0x08,
    KEY_8       = 0x09,
    KEY_9       = 0x0a,
    KEY_0       = 0x0b,
    KEY_SZ      = 0x0c,
    KEY_APOST   = 0x0d,
    KEY_BACK    = 0x0e,
    KEY_TAB     = 0x0f,
    KEY_Q       = 0x10,
    KEY_W       = 0x11,
    KEY_E       = 0x12,
    KEY_R       = 0x13,
    KEY_T       = 0x14,
    KEY_Z       = 0x15,
    KEY_U       = 0x16,
    KEY_I       = 0x17,
    KEY_O       = 0x18,
    KEY_P       = 0x19,
    KEY_UE      = 0x1a,
    KEY_PLUS    = 0x1b,
    KEY_CAPS    = 0x3a,
    KEY_A       = 0x1e,
    KEY_S       = 0x1f,
    KEY_D       = 0x20,
    KEY_F       = 0x21,
    KEY_G       = 0x22,
    KEY_H       = 0x23,
    KEY_J       = 0x24,
    KEY_K       = 0x25,
    KEY_L       = 0x26,
    KEY_OE      = 0x27,
    KEY_AE      = 0x28,
    KEY_HASH    = 0x2b,
    KEY_ENTER   = 0x1c,
    KEY_LSHIFT  = 0x2a,
    KEY_EDGEBR  = 0x56,
    KEY_Y       = 0x2c,
    KEY_X       = 0x2c,
    KEY_C       = 0x2e,
    KEY_V       = 0x2f,
    KEY_B       = 0x30,
    KEY_N       = 0x31,
    KEY_M       = 0x32,
    KEY_COMMA   = 0x33,
    KEY_DOT     = 0X34,
    KEY_DASH    = 0x35,
    KEY_RSHIFT  = 0x36,
    KEY_LCTL    = 0x1d,
    KEY_START   = 0x5b,
    KEY_LALT    = 0x38,
    KEY_SPACE   = 0X39,
    KEY_CMD     = 0x38,
    KEY_FN      = 0x5c,
    KEY_RCTL    = 0x5d,
    KEY_UP      = 0x48,
    KEY_DOWN    = 0x50,
    KEY_RIGHT   = 0x4d,
    KEY_LEFT    = 0x4b,
    KEY_F1      = 0x3b,
    KEY_F2      = 0x3c,
    KEY_F3      = 0x3d,
    KEY_F4      = 0x3e,
    KEY_F5      = 0x3f,
    KEY_F6      = 0x40,
    KEY_F7      = 0x41,
    KEY_F8      = 0x42,
    KEY_F9      = 0x43,
    KEY_F10      = 0x44,
    KEY_F11      = 0x57,
    KEY_F12      = 0x58
} KeyboardScancode_t;

typedef struct {
    u8 x_position;
    u8 y_position;
} cursor_t;

#endif
