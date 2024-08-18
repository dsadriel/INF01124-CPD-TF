#ifndef ARQUIVO_INVERTIDO_H
#define ARQUIVO_INVERTIDO_H

#include "utils.h"
#include "b_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define os tipos de dados utilizados
typedef struct Posting {
    struct Posting *prox;    // Ponteiro para o próximo posting
    keytype *registros;         // Ponteiro para os registros armazenados no posting
} Posting;

typedef struct IndiceInvertido {
    int chave;               // Chave associada ao índice
    long posting;            // Offset do arquivo onde o posting está armazenado
} IndiceInvertido;

typedef struct ArquivoInvertido {
    IndiceInvertido *indices;  // Ponteiro para os índices invertidos
    int __tamanho_posting;     // Tamanho de cada posting
    int __num_indices_max;     // Número máximo de índices
    int num_indices;     // Número de índices em uso
    FILE *arquivo_posting;     // Ponteiro para o arquivo de postings
} ArquivoInvertido;


ArquivoInvertido *criaArquivoInvertido(int num_indices, int tamanho_posting, char *nome_arquivo);
bool liberarArquivoInvertido(ArquivoInvertido *arquivo);
bool liberaPosting(Posting *posting);
Posting *consultaArquivoInvertido(ArquivoInvertido *arquivo, int chave);
IndiceInvertido *__consultaArquivoInvertido(ArquivoInvertido *a, int c, int i, int f);
Posting *carregarPosting_Offset(ArquivoInvertido *arquivo, long offset);
Posting *carregarPosting(ArquivoInvertido *arquivo, IndiceInvertido *indice);


#endif