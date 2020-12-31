TARGET =	bootpart.img
BINPATH =	../buildutils/cross/bin/

ASM = 		nasm
ASMFLAG = 	-I${CURDIR}/src 
CC = 		$(BINPATH)/i686-elf-gcc
CCFLAG = 	-c -m32 -ffreestanding
LD =		$(BINPATH)/i686-elf-ld
LDFLAG = 	-melf_i386 -T$(CURDIR)/i686-elf.x -nostdlib -e main -Ttext 0x8000
OBJCOPY = 	$(BINPATH)/i686-elf-objcopy 
OBJCOPYFLAG = 	-j .text -j .data -j .rodata -j .bss -S -O binary
BSMERGE = 	$(BINPATH)/../../utils/bsmerge
OSNAME := $(shell uname -s)
ifeq ($(OSNAME), Ubuntu)
	MKFSFAT = mkfs.msdos
	MKFSARG = -n "INSTALL" -b 2 -R 32 -s 1 -F 32
endif
ifeq ($(OSNAME), FreeBSD)
	MKFSFAT = newfs_msdos
	MKFSARG = -L "INSTALL" -k 2 -i 1 -r 32 -c 1 -F 32
endif

S1BOOTLDRIMG =	tmp/s1boot.img
S2BOOTLDRBIN =	tmp/s2boot.bin
S2BOOTLDRELF =  tmp/s2boot.elf
S2BOOTLDRRAW =  tmp/s2boot.elf.bin
S2BOOTLDRIMG =  tmp/s2boot.elf.img
FATIMG =	tmp/fat.img

CSOURCES = $(wildcard src/*.c)
CENTRYOBJECT = tmp/main.o
COBJECTS = $(subst tmp/main.o, , $(notdir $(patsubst %.asm, %.o, $(CSOURCEFILES))))
ASMSOURCES = $(subst src/s1boot.asm src/s2boot.asm, , $(wildcard src/*.asm))
ASMOBJECTS = $(notdir $(patsubst %.asm, %.o, $(ASMSOURCES)))

FILES = \
	tmp/* \
	dependency.dep \
	$(FATIMG) \
	$(TARGET)

all : $(TARGET)

mkimage : $(FATIMG)

$(FATIMG) : $(TARGET)
	dd if=/dev/zero of="$(FATIMG)" bs=1024 count=102400
	$(MKFSFAT) $(MKFSARG) "$(FATIMG)"
	#mcopy -i $@ $(S2BOOTLDRBIN) ::STARTUP.BIN
	$(BSMERGE) $@ $(S1BOOTLDRIMG) 0 $(FATIMG)
	$(BSMERGE) $@ $(S2BOOTLDRIMG) 1000 $@
	cp -f $@ $<

$(TARGET) : $(S1BOOTLDRIMG) $(S2BOOTLDRIMG)

$(S2BOOTLDRELF) : $(CENTRYOBJECT) $(COBJECTS) $(ASMOBJECTS)
	$(CC) -MM $(CSOURCES) > dependency.dep
	$(LD) -o $@ $^

tmp/%.o : src/%.asm
	$(ASM) $(ASMFLAG) -felf32 $< -o $@

$(S2BOOTLDRBIN) : $(patsubst tmp/%.bin, src/%.asm, $(S2BOOTLDRBIN))
	$(ASM) $(ASMFLAG) -o $@ $<

$(S1BOOTLDRIMG) : $(patsubst tmp/%.img, src/%.asm, $(S1BOOTLDRIMG))
	$(ASM) $(ASMFLAG) -o $@ $<

$(S2BOOTLDRIMG) : $(S2BOOTLDRBIN) $(S2BOOTLDRELF)
	$(OBJCOPY) $(OBJCOPYFLAG) $(S2BOOTLDRELF) $(S2BOOTLDRRAW)
	cat $(S2BOOTLDRBIN) $(S2BOOTLDRRAW) > $@

tmp/%.o : src/%.c
	$(CC) -c $< -o $@

clean :
	rm -rf $(FILES)
	mkdir -p tmp

ifeq (dependency.dep, $(wildcard dependency.dep))
include dependency.dep
endif
