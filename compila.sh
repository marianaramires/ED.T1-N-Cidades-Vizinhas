#!/bin/sh

echo "Compilando..."

gcc -Wall -pedantic -g -o ./main.exe ./src/hash.c ./src/heap.c ./src/json.c ./src/kdtree.c ./main.c -lm

./main.exe municipios.json