[BITS 64]

global loadGDTR, loadIDTR, loadTR

loadGDTR:
	lgdt[rdi]
	ret

loadTR:
	ltr di
	ret
	
loadIDTR:
	lidt[rdi]
	ret