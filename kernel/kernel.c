typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

void kernel(void) {
    volatile u8* vgabuffer = (volatile u8 *)0xb8000;
    for (u8 i = 0; i < 80*25+100; i++) {
        vgabuffer[i] = i;
        vgabuffer[i+1] = 1;
    }
}
