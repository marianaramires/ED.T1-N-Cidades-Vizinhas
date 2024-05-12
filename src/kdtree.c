#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../include/lib.h"

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