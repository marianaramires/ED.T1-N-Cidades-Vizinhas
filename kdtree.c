#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

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

void *aloca_municipio(char *codigo_ibge, char *nome, float latitude, float longitude,
                      int capital, int codigo_uf, int siafi_id, int ddd, char *fuso_horario)
{
    tmunicipio *municipio = malloc(sizeof(tmunicipio));
    strcpy(municipio->codigo_ibge, codigo_ibge);
    strcpy(municipio->nome, nome);
    municipio->latitude = latitude;
    municipio->longitude = longitude;
    municipio->capital = capital;
    municipio->codigo_uf = codigo_uf;
    municipio->siafi_id = siafi_id;
    municipio->ddd = ddd;
    strcpy(municipio->fuso_horario, fuso_horario);

    return municipio;
}

typedef struct _tnode{
    tmunicipio * reg;
    struct _tnode *esq;
    struct _tnode *dir;
}tnode;

typedef struct _abb{
    tnode * raiz;
    int (*cmp)(void* , void *, int);
}tarv;

int cmp(void *c1, void *c2, int d){
    if (d == 0){
        return ((tmunicipio *) c1)->latitude - ((tmunicipio *) c2)->latitude;
    } else{
        return ((tmunicipio *) c1)->longitude - ((tmunicipio *) c2)->longitude;
    }
}

void kd_constroi(tarv *parv, int (*cmp)(void *, void *, int)){
    parv->raiz = NULL;
    parv->cmp  = cmp;
}

int kd_insere_node(tarv * parv,tnode ** ppnode,void *reg, int i){
    int d = i % 2;
    if (*ppnode == NULL){
         *ppnode = malloc(sizeof(tnode));
         (*ppnode)->reg = reg;
         (*ppnode)->esq = NULL;
         (*ppnode)->dir = NULL;
         printf("inseriu %d\n", d);
    } else if (parv->cmp((*ppnode)->reg,reg,d) > 0){ /* esquerda*/
        kd_insere_node(parv,&((*ppnode)->esq),reg,i+1);
    }else{ /*direita*/
        kd_insere_node(parv,&((*ppnode)->dir),reg,i+1);
    }
}

int kd_insere(tarv * parv,  void * reg){
    return kd_insere_node(parv,&parv->raiz,reg,0);
}

void municipio_printa(tmunicipio municipio)
{
    printf("----- CIDADE -----\n");
    printf("Código IBGE: %s\n", municipio.codigo_ibge);
    printf("Nome: %s\n", municipio.nome);
    printf("Latitude: %f\n", municipio.latitude);
    printf("Longitude: %f\n", municipio.longitude);
    printf("Capital: %d\n", municipio.capital);
    printf("Código UF: %d\n", municipio.codigo_uf);
    printf("ID SIAFI: %d\n", municipio.siafi_id);
    printf("DDD: %d\n", municipio.ddd);
    printf("Fuso Horário: %s\n", municipio.fuso_horario);
}

int main()
{
    tarv arv;
    kd_constroi(&arv, cmp);
    kd_insere(&arv, aloca_municipio("123", "A", 40, 45, 0,
    67, 3, 67, "gmt"));
    kd_insere(&arv, aloca_municipio("123", "B", 15, 70, 0,
    67, 3, 67, "gmt"));
    kd_insere(&arv, aloca_municipio("123", "C", 70, 10, 0,
    67, 3, 67, "gmt"));
    kd_insere(&arv, aloca_municipio("123", "D", 69, 50, 0,
    67, 3, 67, "gmt"));
    kd_insere(&arv, aloca_municipio("123", "E", 66, 85, 0,
    67, 3, 67, "gmt"));
    kd_insere(&arv, aloca_municipio("123", "F", 85, 90, 0,
    67, 3, 67, "gmt"));

    printf("%s\n", arv.raiz->reg->nome); //A
    printf("%s\n", arv.raiz->esq->reg->nome);//B
    printf("%s\n", arv.raiz->dir->reg->nome);//C
    printf("%s\n", arv.raiz->dir->dir->reg->nome);//D
    printf("%s\n", arv.raiz->dir->dir->esq->reg->nome);//E
    printf("%s\n", arv.raiz->dir->dir->dir->reg->nome);//F
    return EXIT_SUCCESS;
}
