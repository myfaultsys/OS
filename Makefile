ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

ASMFLAGS = -f bin
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -Ikernel -Ilib -Imath
LDFLAGS = -m elf_i386 -T linker.ld

all: OS.img
builddir:
	@mkdir -p build
build/%.bin: boot/%.asm | builddir
	$(ASM) $(ASMFLAGS) $< -o $@
build/%.o: kernel/drivers/%.c | builddir
	$(CC) $(CFLAGS) -c $< -o $@
build/kentry.o: kernel/kentry.asm | builddir
	$(ASM) -f elf32 $< -o $@
build/kernel.o: kernel/kernel.c | builddir
	$(CC) $(CFLAGS) -c $< -o $@
build/kernel.elf: build/kentry.o build/kernel.o build/vga.o | builddir
	$(LD) $(LDFLAGS) $^ -o $@
build/kernel.bin: build/kernel.elf | builddir
	$(OBJCOPY) -O binary $< $@
OS.img: build/stage1.bin build/stage2.bin build/kernel.bin | builddir
	cat $^ > $@
run: OS.img
	@qemu-system-i386 -drive format=raw,file=OS.img,if=floppy -boot a
debug: OS.img
	@qemu-system-i386 -drive format=raw,file=OS.img,if=floppy -boot a -s -S
clean:
	@rm -rf build/* OS.img
