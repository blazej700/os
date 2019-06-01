# OS 

Kompilacja:` make`, przy użyciu Cross Compiler https://wiki.osdev.org/GCC_Cross-Compiler

Uruchomienie:`make run`

Uruchomienie z dyskietki:` qemu-system-i386 -fda floppyDisks/os1.img`

Opcjonalnie dla dwóch: ` qemu-system-i386 -fda floppyDisks/os1.img -fdb jakisObraz.img`

Tworzenie dyskietek FAT12: https://wiki.osdev.org/Fat_imgen

### OPIS


#### 1.bootloader i toFatBootloader
  1. boot.s - główny kod bootladera
  2. load_disk.s - ładuje jądro z dysku
  3. switch_to32.s - przechodzi w tryb 32 bitowy
  4. string_16print.s, hex_16print.s - wyświetlanie w trybie 16 bitowym
  5. string_32print.s - wyświetlanie w trybie 32 bitowym
  6. kernel_entry.s - wywołuje główny kod jądra

#### 2.drives
  1. keyboard.c - sterownik klawiatury
  2. timer.c - sterownik timera
  3. screen.c - sterownik vga
  4. interupt.s, isr.c, gdt.s, descriptor_tables.c - obsługa przerwań
  5. input.c - obsługa wejścia
  6. floppy.c - sterownik stacji dyskietek

#### 3.kernel
  1. kernel.c - główny kod jadra
  2. interupt.s, isr.c, descriptor_tables.c - obsługa przerwań

#### 4.utils
  1. ports.c - obsługa portów
  2. util.c - funkcje pomocnicze

### TODO

  - sterownik vga --DONE
  - Przerwania 1 --DONE
  - sterownik klawiatury --DONE
  - Alokator pamieci--DONE
  - Przerwania 2 --DONE
  - Sterownik stacji dyskietek --DONE
  - Obsługa procesów --DONE
  - Fat12 --DONE
  - Zmiany bootloadera do pracy z plikami --DONE
  - Uruchamianie programów --DONE
  - Sterownik dysku twardego
  - Fat16/Fat32 albo Ext2
  - Sterownik USB
