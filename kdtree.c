#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "trabalho.c"

typedef struct _tnode
{
    tmunicipio *reg;
    struct _tnode *esq;
    struct _tnode *dir;
} tnode;

typedef struct _abb
{
    tnode *raiz;
    double (*cmp)(void *, void *, int);
    double (*distancia)(void *, void *);
} tarv;

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

void heapsort(theap *heap){
    int tam = heap->tam;
    for(int i=tam-1; i>0; i--){
        troca(&heap->cidades[0], &heap->cidades[i]);
        heap->tam -=1;
        desce(heap, 0);
    }
    heap->tam = tam;
}

double cmp(void *c1, void *c2, int d)
{
    if (d == 0)
    {
        return ((tmunicipio *)c1)->latitude - ((tmunicipio *)c2)->latitude;
    }
    else
    {
        return ((tmunicipio *)c1)->longitude - ((tmunicipio *)c2)->longitude;
    }
}

double distancia(void *c1, void *c2)
{
    double d = sqrt(pow(cmp(c1, c2, 0), 2) + pow(cmp(c1, c2, 1), 2));
    return d;
}

void kd_constroi(tarv *parv, double (*cmp)(void *, void *, int))
{
    parv->raiz = NULL;
    parv->cmp = cmp;
}

int kd_insere_node(tarv *parv, tnode **ppnode, void *reg, int i)
{
    int d = i % 2;
    if (*ppnode == NULL)
    {
        *ppnode = malloc(sizeof(tnode));
        (*ppnode)->reg = reg;
        (*ppnode)->esq = NULL;
        (*ppnode)->dir = NULL;
    }
    else if (parv->cmp((*ppnode)->reg, reg, d) > 0)
    { /* esquerda*/
        kd_insere_node(parv, &((*ppnode)->esq), reg, i + 1);
    }
    else
    { /*direita*/
        kd_insere_node(parv, &((*ppnode)->dir), reg, i + 1);
    }
}

int kd_insere(tarv *parv, void *reg)
{
    return kd_insere_node(parv, &parv->raiz, reg, 0);
}

void kd_busca_vizinho(tarv *parv, tnode *pnode, tmunicipio *consulta, theap *heap)
{
    double dist;
    int flag;

    if (pnode == NULL)
    {
        return;
    }
    dist = distancia(pnode->reg, consulta);
    if (dist > 0)
    {
        flag = heap_insere_elemento(heap, pnode->reg, dist);
        if (flag == EXIT_FAILURE && dist < heap_acessa_max(*heap))
        {
            heap_altera_prioridade(heap, pnode->reg, dist);
        }
    }
    if (pnode->dir == NULL && pnode->esq == NULL)
    {
        return;
    }

    // busca esquerda
    kd_busca_vizinho(parv, pnode->esq, consulta, heap);
    // busca direita
    kd_busca_vizinho(parv, pnode->dir, consulta, heap);
}

void kd_busca(thash h, tarv *parv, const char *code, theap *heap)
{
    tmunicipio *busca = hash_busca(h, code);
    kd_busca_vizinho(parv, parv->raiz, busca, heap);
}

void heap_print(theap heap){
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
    if(heap == NULL){
        return;
    }

    if(heap->cidades != NULL){
        free(heap->cidades);
        heap->cidades = NULL;
    }

    free(heap);
}