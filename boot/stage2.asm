[bits 16]
[org 0x8000]

%include "boot/macros.inc"

extern _kernel_entry

entry_stage_two:
    cli
    xor ax, ax
    mov ds, ax

    mov [BOOT_DRIVE], dl

    call A20_enable

    mov si, MSG16_A20
    call print_16
    call waiting

    call load_kernel

    mov si, MSG16_KERNEL_LOADED
    call print_16
    call waiting

    lgdt [GDT_descriptor]

    mov si, MSG16_GDT_LOADED
    call print_16
    call waiting

    mov si, MSG16_PROTECTED_MODE_JUMP
    call print_16
    call waiting

    call enable_hardware_protection

    jmp GDT_code_segment_offset:protected_mode

[bits 32]
protected_mode:
    cli
    mov ax, GDT_data_segment_offset
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    cld
    call clear_screen

    mov esi, MSG32_GREETING
    mov edi, FRAME_BUFFER_ADDRESS
    call print_32

    call waiting
    call waiting

    call relocate_kernel

    jmp GDT_code_segment_offset:KERNEL_ADDRESS

    call halt

A20_enable:
    push ax
    xor ax, ax
    in al, FAST_A20_GATE_REGISTER
    or al, 0x02
    out FAST_A20_GATE_REGISTER, al
    pop ax
    ret

load_kernel:
    xor ax, ax
    mov ax, KERNEL_REALMODE_ADDRESS
    mov ax, 0x1000
    mov es, ax
    xor bx, bx
    mov ah, BIOS_DISK_READ_SERVICE
    mov al, KERNEL_SECTOR_COUNT
    mov ch, KERNEL_CYLINDER
    mov dh, KERNEL_HEAD
    mov cl, KERNEL_TARGET_SECTOR
    ;mov dl, [BOOT_DRIVE]
    int BIOS_INTERRUPT_DISK_ACCESS
    jc load_kernel
    ret

fail:
    mov al, ah
    add al, '0'
    mov ah, BIOS_TELETYPE_OUTPUT_FUNCTION
    int BIOS_INTERRUPT_VIDEO_SERVICE
    hlt
    jmp $

enable_hardware_protection:
    push eax
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax
    pop eax
    ret

relocate_kernel:
    mov esi, 0x10000
    mov edi, KERNEL_ADDRESS
    mov ecx, KERNEL_SIZE_IN_BYTES
    rep movsb
    ret

GDT_start:
    dq 0x0
GDT_code_descriptor:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
GDT_data_descriptor:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start

GDT_code_segment_offset equ GDT_code_descriptor - GDT_start
GDT_data_segment_offset equ GDT_data_descriptor - GDT_start

halt:
    cli
    hlt
    jmp halt

print_32:
    mov ah, 0xec
    mov dx, 0x3d4
    mov al, 0x0f
    out dx, al
    mov dx, 0x3d5
    mov al, 26
    out dx, al
    mov dx, 0x3d4
    mov al, 0x0e
    out dx, al
    mov dx, 0x3d5
    mov al, 0
    out dx, al

    lodsb
    test al, al
    jz .end_print_32
    stosw
    jmp print_32
.end_print_32:
    ret

print_32_blank_chars:
    mov ah, 0x00
    lodsb
    test al, al
    jz .end_blank_chars
    stosw
    jmp print_32_blank_chars
.end_blank_chars:
    ret

print_16:
    lodsb
    test al, al
    jz .done
    mov ah, BIOS_TELETYPE_OUTPUT_FUNCTION
    int BIOS_INTERRUPT_VIDEO_SERVICE
    jmp print_16
.done:
    ret

clear_screen:
    mov edi, FRAME_BUFFER_ADDRESS
    mov ecx, VGA_WIDTH * VGA_HEIGHT
    mov ax, 0x0f20
    rep stosw
    ret

waiting:
    push dword 0xffff
    pop ecx
.loop_1:
    push dword 0x02ff
    pop edx
.loop_2:
    dec edx
    jnz .loop_2
    dec ecx
    jnz .loop_1
    ret

BOOT_DRIVE: db 0

MSG32_GREETING: db "CPU ENTERED PROTECTED MODE!", 0
MSG16_A20: db "ENABLED A20 LINE!", NEWLINE, 0
MSG16_KERNEL_LOADED: db "KERNEL LOADED INTO RAM!", NEWLINE, 0
MSG16_GDT_LOADED: db "GLOBAL DESCRIPTOR TABLE LOADED!", NEWLINE, 0
MSG16_PROTECTED_MODE_JUMP: db "JUMPING INTO 32-BIT PROTECTED MODE!", NEWLINE, 0

times 512 - ($-$$) db 0
