global isr_wrapper

extern interrupt_handler

section .text

isr_wrapper:
    pushad
    cld
    call interrupt_handler
    popad
    iret
