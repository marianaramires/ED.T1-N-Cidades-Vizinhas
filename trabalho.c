#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
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
    uintptr_t *table;
    int size;
    int max;
    char *(*get_key)(void *);
    uintptr_t deleted;
} thash;

char *get_key(void *reg)
{
    return (*((tmunicipio *)reg)).codigo_ibge;
}

void aloca_municipio(tmunicipio **municipio, char *codigo_ibge, char *nome, float latitude, float longitude,
                     int capital, int codigo_uf, int siafi_id, int ddd, char *fuso_horario)
{

    strcpy((*municipio)->codigo_ibge, codigo_ibge);
    strcpy((*municipio)->nome, nome);
    (*municipio)->latitude = latitude;
    (*municipio)->longitude = longitude;
    (*municipio)->capital = capital;
    (*municipio)->codigo_uf = codigo_uf;
    (*municipio)->siafi_id = siafi_id;
    (*municipio)->ddd = ddd;
    strcpy((*municipio)->fuso_horario, fuso_horario);
}

char *trata(char *string)
{
    char *pt = strstr(string, ":");
    pt = pt + 2;
    char *virgula = strchr(pt, ',');
    if (virgula)
        *virgula = '\0';

    return pt;
}

void trata_aspas(char *string, char *sem_aspas)
{
    char aspas[500];
    int j = 0;
    strcpy(aspas, trata(string));
    for (int i = 0; aspas[i] != '\0'; i++)
    {
        if (aspas[i] != '"')
        {
            sem_aspas[j++] = aspas[i];
        }
    }
    sem_aspas[j] = '\0';
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

uint32_t hash_duplo(uint32_t h, int max, int i)
{
    int hash = h + (i * ((h % (max - 1)) + 1));
    return hash;
}

int hash_insere(thash *h, void *bucket)
{
    uint32_t hash = hashf(h->get_key(bucket), SEED);
    int pos = hash % (h->max);
    int i = 0;
    // verifica se a tabela esta cheia
    if (h->max == (h->size + 1))
    {
        free(bucket);
        return EXIT_FAILURE;
    }
    else
    { // faz a insercao
        while (h->table[pos] != 0)
        {
            i++;
            if (h->table[pos] == h->deleted)
                break;
            pos = hash_duplo(hash, h->max, i);
        }
        h->table[pos] = (uintptr_t)bucket;
        h->size += 1;
    }
    return EXIT_SUCCESS;
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
    h->get_key = get_key;
    h->deleted = (uintptr_t) & (h->size);
    return EXIT_SUCCESS;
}

void *hash_busca(thash h, const char *key)
{
    uint32_t hash = hashf(key, SEED);
    int pos = hash % (h.max);
    int i = 0;
    void *ret = NULL;
    while (h.table[pos] != 0 && ret == NULL)
    {
        if (strcmp(h.get_key((void *)h.table[pos]), key) == 0)
        {
            ret = (void *)h.table[pos];
        }
        else
        {
            i++;
            pos = hash_duplo(hash, h.max, i);
        }
    }
    return ret;
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
    thash h;
    hash_constroi(&h);
    char c[500];
    char sem_aspas[500];
    long int cont = 0;
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
    arq = fopen("municipios.json", "r");

    fgets(c, 100, arq); // lixo
                        // while (!feof(arq))
    //{
    fgets(c, 100, arq); // lixo

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
    //}

    tmunicipio *temp = malloc(sizeof(tmunicipio));
    aloca_municipio(&temp, codigo_ibge, nome, latitude, longitude, capital,
                    codigo_uf, siafi_id, ddd, fuso_horario);

    assert(hash_insere(&h, temp) == EXIT_SUCCESS);

    tmunicipio *search = hash_busca(h, "5200050");
    if (search != NULL)
    {
        municipio_printa(*search);
    }
    else
    {
        printf("Não encontrado.");
    }

    fclose(arq);
    free(h.table);
    return EXIT_SUCCESS;
}
