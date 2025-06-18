cmake_minimum_required(VERSION 3.20)
project(Nova C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER gcc)

include_directories(/usr/include/efi /usr/include/efi/x86_64)

# Custom build directory names
set(BINARY_DIR ${CMAKE_BINARY_DIR})
set(SOURCE_DIR ${CMAKE_SOURCE_DIR})

# Full build pipeline target
add_custom_target(build_uefi ALL
        COMMAND clang -target x86_64-pc-windows-gnu
        -ffreestanding -fshort-wchar -mno-red-zone
        -I/usr/include/efi -I/usr/include/efi/x86_64
        -c ${SOURCE_DIR}/main.c -o main.o
        # Link ELF
        COMMAND lld-link /subsystem:efi_application /entry:efi_main /out:kernel.efi main.o
        # Create ISO folder
        COMMAND mkdir -p iso/EFI/BOOT
        COMMAND cp kernel.efi iso/EFI/BOOT/BOOTX64.EFI
        COMMAND rm -f fat.img
        COMMAND mkfs.fat -C fat.img 64
        COMMAND mmd -i fat.img ::/EFI
        COMMAND mmd -i fat.img ::/EFI/BOOT
        COMMAND mcopy -i fat.img iso/EFI/BOOT/BOOTX64.EFI ::/EFI/BOOT/BOOTX64.EFI
)

# QEMU run target
add_custom_target(run_qemu
        COMMAND qemu-system-x86_64 -d guest_errors
        -bios /usr/share/edk2-ovmf/x64/OVMF.4m.fd
        -drive file=fat.img,format=raw,if=virtio
        DEPENDS fat.img
        WORKING_DIRECTORY ${BINARY_DIR}
)