; simple-bootloader.asm
bits 16

times 510 - ($-$$) db 0x00
dw 0xAA55
