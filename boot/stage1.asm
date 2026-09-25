[bits 16]
[org 0x7c00]

%include "boot/macros.inc"

entry_stage_one:
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, BOOTLOADER_ENTRY
	mov [BOOT_DRIVE], dl

	call waiting
	call clear_screen
	call waiting
	mov si, MSG_STAGE1_GREETING
	call print_16
	call waiting

disk_reset:
	mov ah, BIOS_DISK_RESET_SERVICE
	int BIOS_INTERRUPT_DISK_ACCESS
	jc disk_reset

	call waiting
	mov si, MSG_DISK_RESET_SUCESS
	call print_16
	call waiting

	mov si, MSG_STAGE2_LOAD
	call print_16
	call waiting

load_second_stage:
	mov ah, BIOS_DISK_READ_SERVICE
	mov al, STAGE2_SECTOR_COUNT
	mov ch, STAGE2_CYLINDER
	mov cl, STAGE2_TARGET_SECTOR
	mov dh, STAGE2_HEAD
	mov bx, STAGE2_ADDRESS
	int BIOS_INTERRUPT_DISK_ACCESS
	jc load_second_stage

	mov si, MSG_STAGE2_IN_RAM
	call print_16
	call waiting

	mov si, MSG_STAGE2_JUMP
	call print_16
	call waiting

second_stage_begin:
	mov dl, [BOOT_DRIVE]
	jmp STAGE2_ADDRESS

print_16:
.handle_character:
	lodsb
	test al, al
	jz .string_ended
	mov ah, BIOS_TELETYPE_OUTPUT_FUNCTION
	int BIOS_INTERRUPT_VIDEO_SERVICE
	jmp .handle_character
.string_ended:
	ret

waiting:
	push 0xffff
	pop cx
.loop_1:
	push 0x00ff
	pop dx
.loop_2:
	dec dx
	jnz .loop_2
	dec cx
	jnz .loop_1
	ret

halt:
	cli
	hlt
	jmp halt

clear_screen:
	mov cx, VGA_WIDTH * VGA_HEIGHT
.loop:
	mov ah, BIOS_TELETYPE_OUTPUT_FUNCTION
	mov al, ' '
	mov bh, 0
	mov bl, 0xbc
	int BIOS_INTERRUPT_VIDEO_SERVICE
	dec cx
	jnz .loop
.endloop:
	ret

BOOT_DRIVE: db 0

MSG_STAGE1_GREETING: db 	"STAGE 1 BOOTLOADER STARTED!", NEWLINE, 0
MSG_DISK_RESET_SUCESS: db 	"DISK SYSTEM RESET SUCCESS!", NEWLINE, 0
MSG_STAGE2_LOAD: db 		"LOADING STAGE 2!", NEWLINE, 0
MSG_STAGE2_IN_RAM: db "STAGE 2 SUCCESSFULLY LOADED INTO RAM!", NEWLINE, 0
MSG_STAGE2_JUMP: db "JUMPING TO STAGE 2 BOOTLOADER...", NEWLINE, 0

times 510 - ($-$$) db 0
dw 0xaa55
