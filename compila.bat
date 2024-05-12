@echo off

echo Compilando...
gcc -pedantic -g -o main.exe ./src/hash.c ./src/heap.c ./src/json.c ./src/kdtree.c ./main.c

START main.exe municipios.json