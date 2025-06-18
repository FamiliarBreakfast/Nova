#include <efi.h>

// NO MORE: Remove efilib.h — you don't need this.

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    (void)ImageHandle;
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello from my UEFI kernel!\r\n");
    while(1){}
    return EFI_SUCCESS;
}
