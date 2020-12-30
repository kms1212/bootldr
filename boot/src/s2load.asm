[ORG 0x00]
[BITS 16]

[SECTION .text]

jmp 0x07C0:_ENTRY

_ENTRY:
	push cs
	pop ds
	jmp $
	
	mov si, String
	call PrintString
	
PrintString:
	pusha
	mov ah, 0x0E
	mov bh, 0x00
.loop:
	lodsb
	cmp al, 0x00
	je .loopend
	int 0x10
	jmp .loop
.loopend :
	popa
	ret
	
[SECTION .data]

String : db 'Hello, World!', 0xA, 0xD, 0