#ifndef H_TYPES
#define H_TYPES

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_CONTROLLER_ADDRESS_PORT 0x3d4
#define VGA_CONTROLLER_DATA_PORT 0x3d5
#define CURSOR_POSITION_SELECT_HIGH_BYTE 0x0e
#define CURSOR_POSITION_SELECT_LOW_BYTE 0x0f

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

#endif
