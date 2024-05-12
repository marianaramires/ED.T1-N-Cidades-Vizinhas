#ifndef __LIB__
#define __LIB__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct _municipio
{
    char codigo_ibge[10];
    char nome[40];
    float latitude;
    float longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[40];
} tmunicipio;

/* HASH */
typedef struct
{
    uintptr_t *table;
    int size;
    int max;
    char *(*get_key)(void *);
    uintptr_t deleted;
} thash;

char *get_cod(void *reg);
char *get_nome(void *reg);
void *aloca_municipio(char *codigo_ibge, char *nome, float latitude, float longitude,
int capital, int codigo_uf, int siafi_id, int ddd, char *fuso_horario);
uint32_t hashf(const char *str, uint32_t h);
uint32_t hash_duplo(uint32_t h, int max, int i);
int hash_insere(thash *h, void *bucket);
int hash_constroi(thash *h, int nbuckets, char *(*get_key)(void *));
void municipio_printa(tmunicipio municipio);
void *hash_busca(thash h, const char *key);
void *hash_busca_nome(thash h, const char *key, int *cont);

/* HEAP */

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

/* KDTREE */
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

/* JSON */
char *trata(char *string);
void trata_aspas(char *string, char *sem_aspas);

#endif