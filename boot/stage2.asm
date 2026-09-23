[bits 16]
[org 0x8000]

%include "boot/macros.inc"

entry_stage_two:
    xor ax, ax
    mov ds, ax
    mov es, ax

    call A20_enable
    call load_kernel
    call load_GDT
    call enable_hardware_protection

A20_enable:
    in al, 0x92
    or al, 0x02
    out 0x92, al
    ret

load_kernel:
    mov ah, BIOS_DISK_READ_SYSTEM
    mov al, KERNEL_SECTOR_COUNT
    mov ch, KERNEL_CYLINDER
    mov dh, KERNEL_HEAD
    mov cl, KERNEL_TARGET_SECTOR
    mov bx, KERNEL_ADDRESS
    int BIOS_INTERRUPT_DISK_ACCESS
    jc load_kernel
    ret

enable_hardware_protection:
    mov eax, cr0
    or ax, 0x1
    mov cr0, ax
    ret

load_GDT:
    lgdt [GDT_descriptor]
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

