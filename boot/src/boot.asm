MBALIGN		equ	1 << 0
MEMINFO		equ 1 << 1	
FLAGS		equ MBALIGN | MEMINFO
MAGIC 		equ	0x1BADB002
CHECKSUM 	equ -(MAGIC + FLAGS)

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bss
align 16
stack_bottom:
	resb 16384
stack_top:

section .text
global _entry:function (_entry.end - _entry)
_entry:
	mov esp, stack_top
	
	extern kern_main
	call kern_main
	
	cli
	
_stop:
	hlt
	jmp _stop

_end: