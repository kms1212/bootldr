[BITS 16]
[ORG 0x00]
[SECTION .text]

_ENTRY:
	jmp short START
	nop

bpbOEMName : DB '__ext~  '
bpbBytesPerSector : DW 0x00
bpbSectorsPerCluster : DB 0x00
bpbReservedSectors : DW 0x00
bpbFATCount : DB 0x00
bpbRootEntries : DW 0x00
bpbTotalSectors : DW 0x00
bpbMedia : DB 0x00 
bpbSectorsPerFAT16 : DW 0x00

bpbSectorsPerTrack : DW 0x00
bpbDiskHeads : DW 0x00
bpbHiddenSectors : DD 0x00
bpbTotalLargeSectors : DD 0x00

ebpbSectorsPerFAT32 : DD 0x00
ebpbFlags : DW 0x00
ebpbFSVersion : DW 0x00
ebpbRootDirectoryCluster : DD 0x00
ebpbFSInfoSector : DW 0x00
ebpbBSBackupSector : DW 0x00
ebpbReserved1 : TIMES 11 DB 0x00
	DB '~'
ebpbPhysicalDriveNum : DB 0x80
ebpbReserved2 : DB '_'
ebpbExtendedBootSign : DB '_'
ebpbVolumeSerial : DB 'ext~'
ebpbVolumeLabel : DB '           '
ebpbFSType : DB '       ~'

START:
	jmp 0x07C0:MAIN
MAIN:
	mov ds, ax
	mov es, ax

	mov ax, 0x1003
	mov bx, 0x00
	int 0x10

	cli
	mov ax, 0x07E0
	mov es, ax
	mov ax, 0x07C0
	mov ds, ax
	mov ax, 0x9000
	mov ss, ax
	mov sp, 0xFFFF
	sti

	mov si, stringBootStart
	call PRINT

	call RESETDRIVE
	mov si, stringDriveInitComplete
	call PRINT
	

	xor bx, bx
	mov cx, 0x18
	mov eax, 0x8
	call READSECTORLBA
	
	jmp 0x07E0:0x0000
	
	jmp $

RESETDRIVE:
	pusha
.RESET:
	mov ah, 0
	int 0x13
	or ah, ah
	jnz .RESET
	popa
	ret

READSECTORLBA:
	pusha
.NEXT :
	pusha
	push byte 0
	push byte 0
	push eax
	push es
	push bx
	push byte 1
	push byte 16

	mov ah, 0x42
	mov dl, [ebpbPhysicalDriveNum]
	mov si, sp
	push ss
	pop ds
	int 0x13
	push cs
	pop ds
	
	jc .ERROR
	add sp, 16

	popa
	dec cx
	jz .END

	add bx, [bpbBytesPerSector]
	add eax, byte 0x01
	jmp .NEXT
.END:
	popa
	ret
.ERROR:
	mov si, stringDriveError
	call PRINT
	jmp $

PRINT:
	pusha
.NEXT:
	lodsb
	or al, al
	jz .END

	mov ah, 0x0E
	xor bx, bx
	int 0x10
	jmp .NEXT
.END:
	popa
	ret

DAP:

stringBootStart : DB 'Bootstrap started', 0x0D, 0x0A, 0x00
stringDriveInitComplete : DB 'Drive initialized', 0x0D, 0x0A, 0x00
stringDriveError : DB 'Drive error', 0x0D, 0x0A, 0x00

TIMES 510 - ( $ - $$ ) DB 0x00
Dw 0xAA55
