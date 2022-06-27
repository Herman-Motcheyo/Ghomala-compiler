#!/bin/bash
output="sortie"
if [ -z "$2" ]
then
  echo ""
else
  output=$2
fi
./parse $1|nasm -f elf -o sortie.o code_assembleur.asm | rm code_assembleur.asm | ld -s -o $output sortie.o -melf_i386 -I/lib/ld-linux.so.2 -lc