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
    int (*distancia)(void *, void *);
} tarv;

typedef struct _vizinho
{
    char *codigo_ibge;
    char *nome;
    double dist;
} tvizinho;

void troca(void *a, void *b)
{
    void *aux;
    aux = a;
    a = b;
    b = aux;
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

void desce(tvizinho *v, int n, int tam)
{
    int maior = n;
    int esq = filho_esq(n);
    int dir = filho_dir(n);

    if (esq < tam && v[esq].dist > v[maior].dist)
        maior = esq;
    if (dir < tam && v[dir].dist > v[maior].dist)
        maior = dir;

    if (maior != n)
    {
        troca(&v[n], &v[maior]);
        desce(v, maior, tam);
    }
}

// int constroi_heap(tvizinho *v, int tam){
//     for (int i = pai(tam); i >= 0; i--)
//     {
//         desce(v, i, tam);
//     }
// }

int sobe(tvizinho *v, int n)
{
    while (v[pai(n)].dist < v[n].dist)
    {
        troca(&v[pai(n)], &v[n]);
        n = pai(n);
    }
}

void altera_prioridade(tvizinho *v, int n, tmunicipio reg, int tam, double dist)
{
    if (v[n].dist < dist)
    {
        v[n].codigo_ibge = reg.codigo_ibge;
        v[n].nome = reg.nome;
        v[n].dist = dist;
        sobe(v, n);
    }
    else if (v[n].dist > dist)
    {
        v[n].codigo_ibge = reg.codigo_ibge;
        v[n].nome = reg.nome;
        v[n].dist = dist;
        desce(v, n, tam);
    }
}

int insere_elemento(tvizinho *v, tmunicipio reg, int *tam, int tam_max, double dist)
{
    int ret = EXIT_SUCCESS;
    if (*tam == tam_max)
        ret = EXIT_FAILURE;
    else
    {
        v[*tam].codigo_ibge = reg.codigo_ibge;
        v[*tam].nome = reg.nome;
        v[*tam].dist = dist;
        sobe(v, *tam);
        *tam = *tam + 1;
    }

    return ret;
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
        printf("inseriu %d\n", d);
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

void kd_busca_vizinho_node(tarv *parv, tnode *pnode, tmunicipio *consulta, double *best)
{
    double dist;

    if (pnode == NULL)
    {
        return;
    }
    else if (strcmp(pnode->reg->codigo_ibge, consulta->codigo_ibge) != 0)
    {
        dist = distancia(pnode->reg, consulta);
        if ((dist < *best) || *best == 0)
        {
            *best = dist;
        }
        if (pnode->dir == NULL && pnode->esq == NULL)
        {
            return;
        }
    }

    // busca esquerda
    kd_busca_vizinho_node(parv, pnode->esq, consulta, best);
    // busca direita
    kd_busca_vizinho_node(parv, pnode->dir, consulta, best);
}

void kd_busca_vizinho(thash h, tarv *parv, const char *code)
{
    tmunicipio *busca = hash_busca(h, code);
    double best = 0;
    kd_busca_vizinho_node(parv, parv->raiz, busca, &best);
    printf("best %f\n", best);
}

int main()
{
    thash h;
    hash_constroi(&h, 11003, get_key);
    char c[500];
    char sem_aspas[500];
    char codigo_ibge[10];
    char nome[40];
    float latitude;
    float longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[40];
    FILE *arq;
    arq = fopen("teste.json", "r");
    char busca[8];
    int condicao = -1;

    tarv arv;
    kd_constroi(&arv, cmp);

    fgets(c, 100, arq); // lixo
    while (!feof(arq))
    {
        fgets(c, 100, arq); // lixo

        if (strchr(c, ']') != NULL)
            break;

        // codigo_ibge
        fgets(c, 500, arq);
        strcpy(codigo_ibge, trata(c));

        // nome
        fgets(c, 500, arq);
        trata_aspas(c, sem_aspas);
        strcpy(nome, sem_aspas);

        // latitude
        fgets(c, 500, arq);
        latitude = atof(trata(c));

        // longitude
        fgets(c, 500, arq);
        longitude = atof(trata(c));

        // capital
        fgets(c, 500, arq);
        capital = atoi(trata(c));

        // codigo_uf
        fgets(c, 500, arq);
        codigo_uf = atoi(trata(c));

        // siafi_id
        fgets(c, 500, arq);
        siafi_id = atoi(trata(c));

        // ddd
        fgets(c, 500, arq);
        ddd = atoi(trata(c));

        // fuso horario
        fgets(c, 500, arq);
        trata_aspas(c, sem_aspas);
        strcpy(fuso_horario, sem_aspas);

        fgets(c, 100, arq); // lixo

        hash_insere(&h, aloca_municipio(codigo_ibge, nome, latitude, longitude, capital,
                                        codigo_uf, siafi_id, ddd, fuso_horario));
        kd_insere(&arv, aloca_municipio(codigo_ibge, nome, latitude, longitude, capital,
                                        codigo_uf, siafi_id, ddd, fuso_horario));
    }

    // kd_insere(&arv, aloca_municipio("1", "A", 40, 45, 0,
    //                                 67, 3, 67, "gmt"));
    // hash_insere(&h, aloca_municipio("1", "A", 40, 45, 0,
    //                                 67, 3, 67, "gmt"));

    // kd_insere(&arv, aloca_municipio("2", "B", 15, 70, 0,
    //                                 67, 3, 67, "gmt"));
    // hash_insere(&h, aloca_municipio("2", "B", 15, 70, 0,
    //                                 67, 3, 67, "gmt"));

    // kd_insere(&arv, aloca_municipio("3", "C", 70, 10, 0,
    //                                 67, 3, 67, "gmt"));
    // hash_insere(&h, aloca_municipio("3", "C", 70, 10, 0,
    //                                 67, 3, 67, "gmt"));

    // kd_insere(&arv, aloca_municipio("4", "D", 69, 50, 0,
    //                                 67, 3, 67, "gmt"));
    // hash_insere(&h, aloca_municipio("4", "D", 69, 50, 0,
    //                                 67, 3, 67, "gmt"));

    // kd_insere(&arv, aloca_municipio("123", "E", 66, 85, 0,
    //                                 67, 3, 67, "gmt"));
    // kd_insere(&arv, aloca_municipio("123", "F", 85, 90, 0,
    //                                 67, 3, 67, "gmt"));
    // kd_insere(&arv, aloca_municipio("123", "G", 60, 80, 0,
    //                                 67, 3, 67, "gmt"));

    // tmunicipio *search = arv.raiz->reg;
    // municipio_printa(*search);
    // printf("%s\n", arv.raiz->reg->nome);                     // A
    // printf("%s\n", arv.raiz->esq->reg->nome);                // B
    // printf("%s\n", arv.raiz->dir->reg->nome);                // C
    // printf("%s\n", arv.raiz->dir->dir->reg->nome);           // D
    // printf("%s\n", arv.raiz->dir->dir->esq->reg->nome);      // E
    // printf("%s\n", arv.raiz->dir->dir->dir->reg->nome);      // F
    // printf("%s\n", arv.raiz->dir->dir->esq->esq->reg->nome); // F

    kd_busca_vizinho(h, &arv, "5200050");
    return EXIT_SUCCESS;
}
