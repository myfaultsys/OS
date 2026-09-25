%define BOOTLOADER_ENTRY                0x7c00
%define BIOS_DISK_RESET_SERVICE         0x00    ;   ah
%define BIOS_DISK_READ_SERVICE          0x02
%define BIOS_INTERRUPT_VIDEO_SERVICE    0x10
%define BIOS_INTERRUPT_DISK_ACCESS      0x13
%define BIOS_TELETYPE_OUTPUT_FUNCTION   0x0e

%define VGA_WIDTH                       80
%define VGA_HEIGHT                      25

%define NEWLINE                         0x0d, 0x0a
%define BACKSPACE                       0x08

%define STAGE2_SECTOR_COUNT             0x02	;	al
%define STAGE2_CYLINDER                 0x00	;	ch
%define STAGE2_HEAD                     0x00	;	dh
%define STAGE2_TARGET_SECTOR            0x02	;	cl
%define STAGE2_ADDRESS                  0x8000	;	bx

%define KERNEL_SECTOR_COUNT             0x06
%define KERNEL_CYLINDER                 0x00
%define KERNEL_HEAD                     0x00
%define KERNEL_TARGET_SECTOR            0x04
%define KERNEL_REALMODE_ADDRESS         0x1000
%define KERNEL_ADDRESS                  0x100000
%define KERNEL_SIZE_IN_BYTES            4096
%define KERNEL_STACK                    0x500000

%define FAST_A20_GATE_REGISTER          0x92
%define FRAME_BUFFER_ADDRESS            0xb8000
