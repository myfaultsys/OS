[bits 32]

%include "boot/macros.inc"

global _kernel_entry
extern kernel

section .text

_kernel_entry:
    mov esp, KERNEL_STACK
    call kernel

hang:
    cli
    hlt
    jmp hang
