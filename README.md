# OS 

kompilacja:` make`, przy użyciu Cross Compiler https://wiki.osdev.org/GCC_Cross-Compiler

uruchomienie:` qemu-system-i386 -fda os-image.bin`

# Dyskietka do odpalenia w VM
1.`dd if=/dev/zero of=os-image.img ibs=1k count=1440`

2.`dd if=os-image.bin of=os-image.img conv=notrunc`

# OPIS

1.Kernel
  1.kernel.c - glowny kod jadra
  2. keyborad.c - sterownik klawiatury
  3. ports.c - obsluga portow
  4. utli.c - pomocnicze funkcje
  5. timer.c - sterownik timer'a
  6. kernel_entry - wywoliuje glowny kod jadra
  7. screen.c - sterownik vga
  8. interupt.s, isr.c, gdt.s, descriptor_tables - obsluga przerwan

2.Bootloader
  1. boot.s - glowny kod bootladera
  2. load_disk.s - laduje jadro z dysku
  3. switch_to32.s - przechodzi w trym 32 bitowy
  4. string_16print.s, hex_16print.s - wyswietlanie w trybie 16 bitowym
  5. string_32print.s - wyswietlanie w trybie 32 bitowym

# To do

  - ~~sterownik vga~~ DONE
  - ~~Przerwania 1~~ DONE
  - ~~sterownik kalwiatury~~ DONE

  - Alokator pamieci
  - Przerwania 2
  - Sterwonik stacji dyskietek
  - Fat12
  - Zmiany bootloadera do pracy z plikami 
  - Uruchamianie progrmów

  - Sterownik twrdego dysku
  - Fat16/Fat32 albo Ext2
  - Sterwonik usb

