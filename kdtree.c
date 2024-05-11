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
           heap.cidades[i].dist,
           heap.cidades[i].municipio->codigo_ibge, heap.cidades[i].municipio->nome, heap.cidades[i].municipio->latitude,
           heap.cidades[i].municipio->longitude, heap.cidades[i].municipio->capital, heap.cidades[i].municipio->codigo_uf,
           heap.cidades[i].municipio->siafi_id, heap.cidades[i].municipio->ddd, heap.cidades[i].municipio->fuso_horario);
    }
    
}

// int main()
// {
//     thash h;
//     hash_constroi(&h, 11003, get_key);
//     char c[500];
//     char sem_aspas[500];
//     char codigo_ibge[10];
//     char nome[40];
//     float latitude;
//     float longitude;
//     int capital;
//     int codigo_uf;
//     int siafi_id;
//     int ddd;
//     char fuso_horario[40];
//     FILE *arq;
//     arq = fopen("municipios.json", "r");
//     char busca[8];
//     int condicao = -1;

//     tarv arv;
//     kd_constroi(&arv, cmp);

//     fgets(c, 100, arq); // lixo
//     while (!feof(arq))
//     {
//         fgets(c, 100, arq); // lixo

//         if (strchr(c, ']') != NULL)
//             break;

//         // codigo_ibge
//         fgets(c, 500, arq);
//         strcpy(codigo_ibge, trata(c));

//         // nome
//         fgets(c, 500, arq);
//         trata_aspas(c, sem_aspas);
//         strcpy(nome, sem_aspas);

//         // latitude
//         fgets(c, 500, arq);
//         latitude = atof(trata(c));

//         // longitude
//         fgets(c, 500, arq);
//         longitude = atof(trata(c));

//         // capital
//         fgets(c, 500, arq);
//         capital = atoi(trata(c));

//         // codigo_uf
//         fgets(c, 500, arq);
//         codigo_uf = atoi(trata(c));

//         // siafi_id
//         fgets(c, 500, arq);
//         siafi_id = atoi(trata(c));

//         // ddd
//         fgets(c, 500, arq);
//         ddd = atoi(trata(c));

//         // fuso horario
//         fgets(c, 500, arq);
//         trata_aspas(c, sem_aspas);
//         strcpy(fuso_horario, sem_aspas);

//         fgets(c, 100, arq); // lixo

//         hash_insere(&h, aloca_municipio(codigo_ibge, nome, latitude, longitude, capital,
//                                         codigo_uf, siafi_id, ddd, fuso_horario));
//         kd_insere(&arv, aloca_municipio(codigo_ibge, nome, latitude, longitude, capital,
//                                         codigo_uf, siafi_id, ddd, fuso_horario));
//     }

//     // kd_insere(&arv, aloca_municipio("1", "A", 40, 45, 0,
//     //                                 67, 3, 67, "gmt"));
//     // hash_insere(&h, aloca_municipio("1", "A", 40, 45, 0,
//     //                                 67, 3, 67, "gmt"));

//     // kd_insere(&arv, aloca_municipio("2", "B", 15, 70, 0,
//     //                                 67, 3, 67, "gmt"));
//     // hash_insere(&h, aloca_municipio("2", "B", 15, 70, 0,
//     //                                 67, 3, 67, "gmt"));

//     // kd_insere(&arv, aloca_municipio("3", "C", 70, 10, 0,
//     //                                 67, 3, 67, "gmt"));
//     // hash_insere(&h, aloca_municipio("3", "C", 70, 10, 0,
//     //                                 67, 3, 67, "gmt"));

//     // kd_insere(&arv, aloca_municipio("4", "D", 69, 50, 0,
//     //                                 67, 3, 67, "gmt"));
//     // hash_insere(&h, aloca_municipio("4", "D", 69, 50, 0,
//     //                                 67, 3, 67, "gmt"));

//     // kd_insere(&arv, aloca_municipio("123", "E", 66, 85, 0,
//     //                                 67, 3, 67, "gmt"));
//     // kd_insere(&arv, aloca_municipio("123", "F", 85, 90, 0,
//     //                                 67, 3, 67, "gmt"));
//     // kd_insere(&arv, aloca_municipio("123", "G", 60, 80, 0,
//     //                                 67, 3, 67, "gmt"));

//     // tmunicipio *search = arv.raiz->reg;
//     // municipio_printa(*search);
//     // printf("%s\n", arv.raiz->reg->nome);                     // A
//     // printf("%s\n", arv.raiz->esq->reg->nome);                // B
//     // printf("%s\n", arv.raiz->dir->reg->nome);                // C
//     // printf("%s\n", arv.raiz->dir->dir->reg->nome);           // D
//     // printf("%s\n", arv.raiz->dir->dir->esq->reg->nome);      // E
//     // printf("%s\n", arv.raiz->dir->dir->dir->reg->nome);      // F
//     // printf("%s\n", arv.raiz->dir->dir->esq->esq->reg->nome); // F

//     theap heap;
//     heap_constroi(&heap, 10);
//     printf("Oi\n");
//     kd_busca(h, &arv, "5002704", &heap);
//     printf("depois de busca\n");
//     heapsort(&heap);
//     heap_print(heap);
//     return EXIT_SUCCESS;
// }
