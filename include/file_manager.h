#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdio.h>
#include <entidades.h>
#include <stdlib.h>

#define NUM_ARQUIVOS 4

extern FILE *ARQUIVO_DADOS[NUM_ARQUIVOS];
extern FILE *ARQUIVO_INDICES[NUM_ARQUIVOS];
extern char *NOMES_ARQUIVOS[NUM_ARQUIVOS];
extern char *NOMES_INDICES[NUM_ARQUIVOS];

#define ORDEM_ARVORE_PACIENTES 50     // Ordem da árvore B de pacientes
#define ORDEM_ARVORE_AGENDAMENTOS 100 // Ordem da árvore B de agendamentos


typedef struct arquivo_aberto {
    FILE *arquivo;
    char *nome;
    struct arquivo_aberto *proximo;
} ArquivosAbertos;


void finalizar_file_manager();

size_t obter_indice_arquivo(size_t id, TipoEntidade tipo);
FILE *obter_arquivo_dados(TipoEntidade tipo, bool criar);
FILE *obter_arquivo_indices(TipoEntidade tipo, bool criar);

Agendamento *ler_agendamento_disco(size_t id);
Paciente *ler_paciente_disco(size_t id);

void apagar_todos_os_dados();

size_t fappend(const void *content, size_t size, FILE *file);

#endif