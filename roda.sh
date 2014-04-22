#!/bin/bash
clear

gcc -g -o lzw lzw.c
./lzw -c teste.in > temp1
./lzw -d teste.lzw > temp2

diff teste.in teste.txt

echo "tamanho do arquivo original: "
du -b teste.in | cut -f1


echo "tamanho do arquivo compactado: "
du -b teste.lzw | cut -f1


echo "tamanho do arquivo descompactado: "
du -b teste.txt | cut -f1

echo -e "diferenca entre os dicionarios:\n"
diff temp1 temp2 -y

rm -f temp*