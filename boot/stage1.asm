[bits 16]
[org 0x7c00]

%include "boot/macros.inc"

entry_stage_one:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, BOOTLOADER_ENTRY
	mov [BOOT_DRIVE], dl

mov si, MSG_STAGE1_GREETING
call print_16

disk_reset:
	mov ah, BIOS_DISK_RESET_SERVICE
	int BIOS_INTERRUPT_DISK_ACCESS
	jc disk_reset

mov si, MSG_DISK_RESET_SUCESS
call print_16

mov si, MSG_STAGE2_LOAD
call print_16

load_second_stage:
	mov ah, BIOS_DISK_READ_SERVICE
	mov al, STAGE2_SECTOR_COUNT
	mov ch, STAGE2_CYLINDER
	mov cl, STAGE2_TARGET_SECTOR
	mov dh, STAGE2_HEAD
	mov bx, STAGE2_ADDRESS
	int BIOS_INTERRUPT_DISK_ACCESS
	jc load_second_stage

second_stage_begin:
	jmp 0x0000:STAGE2_ADDRESS

print_16:
	call waiting
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
	mov cx, 0x0fff
.loop_1:
	mov dx, 0xffff
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

MSG_STAGE1_GREETING: db 	"STAGE 1 BOOTLOADER STARTED!", NEWLINE, 0
MSG_DISK_RESET_SUCESS: db 	"DISK SYSTEM RESET SUCCESS!", NEWLINE, 0
MSG_STAGE2_LOAD: db 		"LOADING STAGE 2!", NEWLINE, 0

BOOT_DRIVE: db 0

times 510 - ($-$$) db 0
dw 0xaa55
