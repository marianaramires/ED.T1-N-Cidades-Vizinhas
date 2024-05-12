#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../include/lib.h"

void troca(tvizinho *a, tvizinho *b)
{
    tvizinho aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

int pai(int n)
{
    return ((n - 1) / 2);
}

int filho_esq(int n)
{
    return 2 * n + 1;
}

int filho_dir(int n)
{
    return 2 * n + 2;
}

void desce(theap *heap, int n)
{
    int maior = n;
    int esq = filho_esq(n);
    int dir = filho_dir(n);

    if (esq < heap->tam && heap->cidades[esq].dist > heap->cidades[maior].dist)
        maior = esq;
    if (dir < heap->tam && heap->cidades[dir].dist > heap->cidades[maior].dist)
        maior = dir;
    if (maior != n)
    {
        troca(&heap->cidades[n], &heap->cidades[maior]);
        desce(heap, maior);
    }
}

int sobe(theap *heap, int n)
{
    while (heap->cidades[pai(n)].dist < heap->cidades[n].dist)
    {
        troca(&heap->cidades[pai(n)], &heap->cidades[n]);
        n = pai(n);
    }
}

void heap_altera_prioridade(theap *heap, tmunicipio *reg, double dist)
{
    heap->cidades[0].municipio = reg;
    heap->cidades[0].dist = dist;
    desce(heap, 0);
}

int heap_insere_elemento(theap *heap, tmunicipio *reg, double dist)
{
    int ret = EXIT_SUCCESS;
    if ((heap->tam) == heap->max)
    {
        ret = EXIT_FAILURE;
    }
    else
    {
        heap->cidades[heap->tam].municipio = reg;
        heap->cidades[heap->tam].dist = dist;
        sobe(heap, heap->tam);
        heap->tam += 1;
    }
    return ret;
}

double heap_acessa_max(theap heap)
{
    return heap.cidades[0].dist;
}

void heap_constroi(theap *heap, int max)
{
    heap->cidades = (tvizinho *)malloc(max * sizeof(tvizinho *));
    heap->tam = 0;
    heap->max = max;
}

void heapsort(theap *heap)
{
    int tam = heap->tam;
    for (int i = tam - 1; i > 0; i--)
    {
        troca(&heap->cidades[0], &heap->cidades[i]);
        heap->tam -= 1;
        desce(heap, 0);
    }
    heap->tam = tam;
}

void heap_print(theap heap)
{
    int tam = heap.tam;
    printf("%-9s | %-12s | %-30s | %-10s | %-10s | %-6s | %-3s | %-5s | %-3s | %-3s\n",
           "Distancia", "Codigo IBGE", "Nome", "Latitude", "Longitude",
           "Capital", "Codigo UF", "ID SIAFI", "DDD", "Fuso Horario");
    for (int i = 0; i < tam; i++)
    {
        printf("%-9f | %-12s | %-30s | %-10f | %-10f | %-6d | %-3d | %-5d | %-3d | %-3s\n",
               heap.cidades[i].dist * 100,
               heap.cidades[i].municipio->codigo_ibge, heap.cidades[i].municipio->nome, heap.cidades[i].municipio->latitude,
               heap.cidades[i].municipio->longitude, heap.cidades[i].municipio->capital, heap.cidades[i].municipio->codigo_uf,
               heap.cidades[i].municipio->siafi_id, heap.cidades[i].municipio->ddd, heap.cidades[i].municipio->fuso_horario);
    }
}

void heap_destroi(theap * heap){
    if (heap == NULL) {
        return;
    }

    // Libere a memória de cada elemento do vetor de cidades
    for (int i = 0; i < heap->tam; i++) {
        free(heap->cidades[i].municipio);
        heap->cidades[i].dist = 0;
    }

    free(heap->cidades);
    free(heap);
}