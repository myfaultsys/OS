global isr_wrapper
global isr_stub_table

extern interrupt_handler

section .text

isr_wrapper:
    pushad
    cld
    call interrupt_handler
    popad
    add esp, 0x08
    iret

%macro ISR_NOERR 1
    global isr%1
    isr%1:
        cli
        push dword 0
        push dword %1
        jmp isr_wrapper
%endmacro

%macro ISR_ERR 1
    global isr%1
    isr%1:
        cli
        push dword 1
        push dword %1
        jmp isr_wrapper
%endmacro

%assign i 0
%rep 256
    %if i == 8 || i == 10 || i == 11 || i == 12 || i == 13 || i == 14 || i == 17 || i == 21
        ISR_ERR i
    %else
        ISR_NOERR i
    %endif
    %assign i i+1
%endrep


section .data

global isr_stub_table

isr_stub_table:
%assign i 0
%rep 256
    dd isr%+i
    %assign i i+1
%endrep
