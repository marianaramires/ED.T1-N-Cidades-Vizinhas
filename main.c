#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "include/lib.h"

int main()
{
    thash h;
    hash_constroi(&h, 11003, get_cod);

    thash h_nome;
    hash_constroi(&h_nome, 11003, get_nome);

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
    arq = fopen("municipios.json", "r");
    char busca[8];
    int condicao = -1;
    int vizinhos;

    tarv arv;
    kd_constroi(&arv, cmp);

    theap heap;

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
        hash_insere(&h_nome, aloca_municipio(codigo_ibge, nome, latitude, longitude, capital,
                                             codigo_uf, siafi_id, ddd, fuso_horario));
        kd_insere(&arv, aloca_municipio(codigo_ibge, nome, latitude, longitude, capital,
                                        codigo_uf, siafi_id, ddd, fuso_horario));
    }

    while (condicao != 0)
    {
        printf("___________________________________________________________________________\n");
        printf("1 - Consulte uma cidade pelo seu codigo IBGE\n");
        printf("2 - Consulte os N vizinhos mais proximos de uma cidade pelo seu codigo IBGE\n");
        printf("3 - Consulte os N vizinhos mais proximos de uma cidade pelo seu nome \n");
        printf("0 - Sair\n");
        scanf("%d", &condicao);

        switch (condicao)
        {
        case 1:
            printf("\nDigite o codigo IBGE da cidade: ");
            scanf("%s", busca);

            tmunicipio *search = hash_busca(h, busca);
            if (search != NULL)
            {
                municipio_printa(*search);
            }
            else
            {
                printf("Cidade não encontrada.");
            }
            break;
        case 2:
            printf("\nDigite o codigo IBGE da cidade: ");
            scanf("%s", busca);

            printf("Digite quantos vizinhos deseja buscar: ");
            scanf("%d", &vizinhos);
            heap_constroi(&heap, vizinhos);

            kd_busca(h, &arv, busca, &heap);
            heapsort(&heap);
            heap_print(heap);
            break;
        case 3:
            printf("\nDigite o nome da cidade: ");
            scanf(" %[^\n]s", busca);
            int j = 0;

            tmunicipio *search_nome = hash_busca_nome(h_nome, busca, &j);
            if (search_nome != NULL)
            {
                if (j > 1)
                {
                    printf("Mais de uma cidade encontrada. Digite o codigo IBGE da cidade desejada: ");
                    scanf("%s", busca);

                    search_nome = hash_busca(h, busca);
                }

                printf("Digite quantos vizinhos deseja buscar: ");
                scanf("%d", &vizinhos);
                heap_constroi(&heap, vizinhos);

                kd_busca(h, &arv, search_nome->codigo_ibge, &heap);
                heapsort(&heap);
                heap_print(heap);
            }
            else
            {
                printf("Cidade não encontrada.");
            }
            break;
        case 0:
            break;
        default:
            printf("Essa não é uma opção válida, digite novamente");
            break;
        }
    }

    fclose(arq);
    free(h.table);
    free(h_nome.table);
    return EXIT_SUCCESS;
}