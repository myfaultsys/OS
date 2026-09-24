[bits 32]

global _kernel_entry
extern kernel

section .text

_kernel_entry:
    mov esp, 0x90000
    call kernel

hang:
    cli
    hlt
    jmp hang
