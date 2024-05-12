#ifndef __HEAP__
#define __HEAP__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "municipio.h"

typedef struct _vizinho
{
    tmunicipio *municipio;
    double dist;
} tvizinho;

typedef struct _heap
{
    tvizinho *cidades;
    int tam;
    int max;
} theap;

void troca(tvizinho *a, tvizinho *b);

int pai(int n);
int filho_esq(int n);
int filho_dir(int n);
void desce(theap *heap, int n);
int sobe(theap *heap, int n);
void heap_altera_prioridade(theap *heap, tmunicipio *reg, double dist);
int heap_insere_elemento(theap *heap, tmunicipio *reg, double dist);
double heap_acessa_max(theap heap);
void heap_constroi(theap *heap, int max);
void heapsort(theap *heap);
void heap_print(theap heap);

#endif