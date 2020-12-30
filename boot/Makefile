TARGET =	bootpart.img
ASM = 		nasm
ASMFLAGS = 	-I${CURDIR}/src
CC = 		gcc

S1BOOTLDRBIN =	tmp/s1boot.bin
S2LOADLDRBIN =	tmp/s2load.bin
S2BOOTLDRBIN =	tmp/s2boot.bin
FATIMG =	src/fat.img

FILES = \
	tmp/* \
	$(TARGET)

all : $(FATIMG) $(TARGET)

$(FATIMG) : 
	dd if=/dev/zero of=./fat.img bs=1024 count=102400
	mkfs.fat -F 32 fat.img

$(TARGET) : $(S1BOOTLDRBIN) $(S2BOOTLDRBIN) $(S2LOADLDRBIN)
	../buildutils/bsmerge/bsmerge $@ $(S1BOOTLDRBIN) 0 $(FATIMG)
	../buildutils/bsmerge/bsmerge $@ $(S2LOADLDRBIN) 1000 $@
	mcopy -i $@ $(S2BOOTLDRBIN) ::STARTUP.BIN

tmp/%.bin : src/%.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean :
	rm -rf $(FILES)
