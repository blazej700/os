# OS 

kompilacja:` make`, przy użyciu Cross Compiler https://wiki.osdev.org/GCC_Cross-Compiler

uruchomienie:` qemu-system-i386 -fda os-image.bin`

# Dyskietka do odpalenia w VM
1.`dd if=/dev/zero of=os-image.img ibs=1k count=1440`

2.`dd if=os-image.bin of=os-image.img conv=notrunc`

# To do

  - sterownik vga
  - Przerwania 1
  - sterownik kalwiatury

  - Alokator pamieci
  - Przerwania 2
  - Sterwonik stacji dyskietek
  - Fat12
  - Zmiany bootloadera do pracy z plikami 
  - Uruchamianie progrmów

  - Sterownik twrdego dysku
  - Fat16/Fat32 albo Ext2
  - Sterwonik usb

