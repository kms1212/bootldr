[ORG 0x00]
[BITS 16]

[SECTION .text]

VMEMADDR	equ	0xB800

jmp 0x0060:_ENTRY

_ENTRY:
	push cs
	pop ds
	
	mov si, Stage2
	call PrintString
 
	mov si, DAPACK		; address of "disk address packet"
	mov ah, 0x42		; AL is unused
	
	int 0x13
	
	mov si, msg2
	call PrintString
	
	jc short .error

	jmp 0x07C0:0000
	
.error:
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

String : db 'Error reading drive.', 0xA, 0xD, 0
Stage2 : db 'Stage 2', 0xA, 0xD, 0
msg1 : db 'pre-int', 0xA, 0xD, 0
msg2 : db 'post-int', 0xA, 0xD, 0


DAPACK:
	db	0x10
	db	0
blkcnt:	
	dw	16		; int 13 resets this to # of blocks actually read/written
db_add:	
	dw	0x7C00		; memory buffer destination address (0:7c00)
	dw	0		; in memory page zero
d_lba:	
	dd	8		; put the lba to read in this spot
	dd	0		; more storage bytes only for big lba's ( > 4 bytes )