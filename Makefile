ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

ASMFLAGS = -f bin
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector
LDFLAGS = -m elf32_i386 -T linker.ld

all: OS.img
builddir:
	@mkdir -p build
build/stage1.bin: boot/stage1.asm | builddir
	$(ASM) $(ASMFLAGS) $< -o $@
OS.img: build/stage1.bin | builddir
	cat $^ > $@
run: OS.img
	@qemu-system-i386 -drive format=raw,file=OS.img,if=floppy -boot a
clean:
	@rm -rf build/* OS.img
