#ifndef __LIB__
#define __LIB__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "municipio.h"
#include "heap.h"
#include "hash.h"

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

void kd_constroi(tarv *parv, double (*cmp)(void *, void *, int));
int kd_insere_node(tarv *parv, tnode **ppnode, void *reg, int i);
int kd_insere(tarv *parv, void *reg);
void kd_busca_vizinho(tarv *parv, tnode *pnode, tmunicipio *consulta, theap *heap);
void kd_busca(thash h, tarv *parv, const char *code, theap *heap);
double cmp(void *c1, void *c2, int d);
double distancia(void *c1, void *c2);

#endif