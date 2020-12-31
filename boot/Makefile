TARGET =	bootpart.img
ASM = 		nasm
ASMFLAGS = 	-I${CURDIR}/src
CC = 		gcc

OSNAME := $(shell uname -s)

ifeq ($(OSNAME), Ubuntu)
	MKFSFAT = -n "INSTALL" -b 2 -R 32 -s 1 mkfs.msdos
	MKFSARG = -F 32
endif
ifeq ($(OSNAME), FreeBSD)
	MKFSFAT = newfs_msdos
	MKFSARG = -L "INSTALL" -k 2 -i 1 -r 32 -c 1 -F 32
endif

S1BOOTLDRBIN =	tmp/s1boot.bin
S2BOOTLDRBIN =	tmp/s2boot.bin
FATIMG =	tmp/fat.img

FILES = \
	tmp/* \
	$(FATIMG) \
	$(TARGET)

all : $(TARGET)

mkimage : $(FATIMG)

$(FATIMG) : $(TARGET)
	dd if=/dev/zero of="$(FATIMG)" bs=1024 count=102400
	$(MKFSFAT) $(MKFSARG) "$(FATIMG)"
	#mcopy -i $@ $(S2BOOTLDRBIN) ::STARTUP.BIN
	../buildutils/bsmerge/bsmerge $@ $(S1BOOTLDRBIN) 0 $(FATIMG)
	../buildutils/bsmerge/bsmerge $@ $(S2BOOTLDRBIN) 1000 $@
	cp -f $@ $<

$(TARGET) : $(S1BOOTLDRBIN) $(S2BOOTLDRBIN)

tmp/%.bin : src/%.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean :
	rm -rf $(FILES)
	mkdir -p tmp
