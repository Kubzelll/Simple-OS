#!/bin/bash

build_kernel() {
    # Assemble boot.s file
    as --32 boot.s -o boot.o
    if [ $? -ne 0 ]; then
        echo "Error assembling boot.s. Exiting."
        exit 1
    fi

    gcc -m32 -c kernel/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
    if [ $? -ne 0 ]; then
        echo "Error compiling kernel.c. Exiting."
        exit 1
    fi
    ld -m elf_i386 -T linker.ld kernel.o boot.o -o MyOS.bin
    if [ $? -ne 0 ]; then
        echo "Error linking kernel. Exiting."
        exit 1
    fi

    mkdir -p isodir/boot/grub
    cp MyOS.bin isodir/boot/MyOS.bin
    cp grub.cfg isodir/boot/grub/grub.cfg
    grub-mkrescue -o MyOS.iso isodir
    if [ $? -ne 0 ]; then
        echo "Error building ISO. Exiting."
        exit 1
    fi

    echo "Os built successfully."
}

echo "Do you want to (1) build kernel only or (2) build and run?"
read choice

case $choice in
    1)
        build_kernel
        ;;

    2)
        build_kernel
        qemu-system-x86_64 -cdrom MyOS.iso
        exit $?
        ;;

    *)
        echo "Invalid choice. Exiting."
        exit 1
        ;;
esac

exit $?
