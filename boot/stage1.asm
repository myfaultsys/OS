[bits 16]
[org 0x7c00]

%include "boot/macros.inc"

entry_stage1:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7c00
