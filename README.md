# OS 

kompilacja:` make`, przy użyciu Cross Compiler https://wiki.osdev.org/GCC_Cross-Compiler

uruchomienie:` qemu-system-i386 -fda os-image.bin`

# Dyskietka do odpalenia w VM
1.`dd if=/dev/zero of=os-image.img ibs=1k count=1440`

2.`dd if=os-image.bin of=os-image.img conv=notrunc`

# OPIS

1.Kernel
  1. kernel.c - główny kod jadra
  2. keyboard.c - sterownik klawiatury
  3. ports.c - obsługa portów
  4. util.c - funkcje pomocnicze
  5. timer.c - sterownik timera
  6. kernel_entry.s - wywołuje główny kod jądra
  7. screen.c - sterownik vga
  8. interupt.s, isr.c, gdt.s, descriptor_tables.c - obsługa przerwań
  9. input.c - obsługa wejścia

2.Bootloader
  1. boot.s - główny kod bootladera
  2. load_disk.s - ładuje jądro z dysku
  3. switch_to32.s - przechodzi w tryb 32 bitowy
  4. string_16print.s, hex_16print.s - wyświetlanie w trybie 16 bitowym
  5. string_32print.s - wyświetlanie w trybie 32 bitowym

# TODO

  - ~~sterownik vga --DONE
  - ~~Przerwania 1 --DONE
  - ~~sterownik kalwiatury --DONE

  - Alokator pamieci Częściowo DONE
  - ~~Przerwania 2 --DONE
  - Sterownik stacji dyskietek --DONE
  - Obsługa procesów --DONE
  - Fat12
  - Zmiany bootloadera do pracy z plikami 
  - Uruchamianie programów

  - Sterownik dysku twardego
  - Fat16/Fat32 albo Ext2
  - Sterownik USB

