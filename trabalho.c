#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define SEED 0x12345678

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

typedef struct
{
    tmunicipio *table;
    int size;
    int max;
} thash;

char* trata(char *string){
    char *pt = strstr(string, ":");
    pt=pt+2;
    char *virgula = strchr(pt, ',');
    if(virgula)
        *virgula = '\0';

    return pt;
}

void trata_aspas(char *string, char *sem_aspas){
    char aspas[500];
    int j=0;
    strcpy(aspas, trata(string));
    for (int i = 0; aspas[i]!='\0'; i++)
    {
        if(aspas[i] != '"'){
            sem_aspas[j++]=aspas[i];
        }
    }
    sem_aspas[j]='\0';
}

uint32_t hashf(const char *str, uint32_t h)
{
    /* One-byte-at-a-time Murmur hash
    Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    for (; *str; ++str)
    {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

int hash_constroi(thash *h)
{
    // 11003 por ser quase o dobro da qtnde de elementos e ser numero primo
    h->table = calloc(sizeof(void *), 11003);
    if (h->table == NULL)
    {
        return EXIT_FAILURE;
    }
    h->max = 11003;
    h->size = 0;
    return EXIT_SUCCESS;
}

int main()
{
    thash h;
    hash_constroi(&h);
    char c[500];
    char sem_aspas[500];
    long int cont=0;
    FILE *arq;
    arq = fopen("municipios.json", "r");

    fgets(c, 100, arq); // lixo
    while (!feof(arq))
    {
        fgets(c, 100, arq); // lixo

        // codigo_ibge
        fgets(c, 500, arq); 
        strcpy(h.table[0].codigo_ibge, trata(c));
        printf("%s\n", h.table[0].codigo_ibge);

        // nome
        fgets(c, 500, arq); 
        trata_aspas(c,sem_aspas);
        strcpy(h.table[0].nome, sem_aspas);
        printf("%s\n", h.table[0].nome);

        // latitude
        fgets(c, 500, arq); 
        h.table[0].latitude = atof(trata(c));
        printf("%f\n", h.table[0].latitude);

        // longitude
        fgets(c, 500, arq); 
        h.table[0].longitude = atof(trata(c));
        printf("%f\n", h.table[0].longitude);
        
        // capital
        fgets(c, 500, arq); 
        h.table[0].capital = atoi(trata(c));
        printf("%d\n", h.table[0].capital);

        // codigo_uf
        fgets(c, 500, arq); 
        h.table[0].codigo_uf = atoi(trata(c));
        printf("%d\n", h.table[0].codigo_uf);

        // siafi_id
        fgets(c, 500, arq); 
        h.table[0].siafi_id = atoi(trata(c));
        printf("%d\n", h.table[0].siafi_id);

        // ddd
        fgets(c, 500, arq); 
        h.table[0].ddd = atoi(trata(c));
        printf("%d\n", h.table[0].ddd);

        // fuso horario
        fgets(c, 500, arq); 
        trata_aspas(c,sem_aspas);
        strcpy(h.table[0].fuso_horario, sem_aspas);
        printf("%s\n", h.table[0].fuso_horario);

        fgets(c, 100, arq); // lixo
        cont++;
        printf("Oi");
        printf("%d\n", cont);
    }

    fclose(arq);
    free(h.table);
    return EXIT_SUCCESS;
}
