# OS 

kompilacja:` make`

uruchomienie:` qemu-system-i386 -fda os-image.bin`

# Dyskietka do odpalenia w VM
1.`dd if=/dev/zero of=floppy.img ibs=1k count=1440`

2.`dd if=os-image.bin of=os-image.img conv=notrunc`

# To do

  - ~~Cross Compiler~~ zrobione za pomocą https://wiki.osdev.org/GCC_Cross-Compiler
  - String
  - print w c
  - obsługa klawiatury
