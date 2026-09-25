#ifndef H_VGA
#define H_VGA

void spawn_char_vga(u8 posx, u8 posy, i8 character, u8 color);
void vga_ctl_set_low_byte(u8 byte);
void vga_ctl_set_high_byte(u8 byte);
void cursor_set_position(u8 x, u8  y);
void clear_screen(void);
void drawing(void);

#endif
