[bits 16]
[org 0x8000]

%include "boot/macros.inc"

extern _entry

entry_stage_two:
    xor ax, ax
    mov ds, ax

    call A20_enable
    call load_kernel
    lgdt [GDT_descriptor]
    call enable_hardware_protection

    jmp GDT_code_segment_offset:protected_mode

[bits 32]
protected_mode:
    mov ax, GDT_data_segment_offset
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x9000
    cld

    call clear_screen
    mov esi, MSG32_GREETING
    mov edi, FRAME_BUFFER_ADDRESS
    call print_32

    call halt
    call relocate_kernel

A20_enable:
    in al, FAST_A20_GATE_REGISTER
    or al, 0x02
    out FAST_A20_GATE_REGISTER, al
    ret

load_kernel:
    mov ax, KERNEL_REALMODE_ADDRESS
    mov es, ax
    xor bx, bx
    mov ah, BIOS_DISK_READ_SERVICE
    mov al, KERNEL_SECTOR_COUNT
    mov ch, KERNEL_CYLINDER
    mov dh, KERNEL_HEAD
    mov cl, KERNEL_TARGET_SECTOR
    int BIOS_INTERRUPT_DISK_ACCESS
    jc load_kernel
    ret

enable_hardware_protection:
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    ret

relocate_kernel:
    mov esi, KERNEL_REALMODE_ADDRESS
    mov edi, KERNEL_ADDRESS
    mov ecx, KERNEL_SIZE_IN_BYTES   ; immediate value required here
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
    dw  0xffff
    dw  0x0
    db  0x0
    db  10010010b
    db  11001111b
    db  0x0
GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start

GDT_code_segment_offset equ GDT_code_descriptor - GDT_start
GDT_data_segment_offset equ GDT_data_descriptor - GDT_start

halt:
    hlt
    jmp halt

print_32:
    mov ah, 0xec
    mov dx, 0x3d4
    mov al, 0x0f
    out dx, al
    mov dx, 0x3d5
    mov al, 34
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

clear_screen:
    mov edi, 0xb8000
    mov ecx, 80*25
    mov ax, 0x0f20
    rep stosw
    ret

MSG32_GREETING: db "CPU ENTERED PROTECTED MODE!", 0

times 511 - ($-$$) db 0
