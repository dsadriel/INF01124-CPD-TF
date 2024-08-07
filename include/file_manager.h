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

bool iniciar_file_manager();
void finalizar_file_manager();

size_t obter_indice_arquivo(size_t id, TipoEntidade tipo);
FILE *obter_arquivo_dados(TipoEntidade tipo);

Agendamento *ler_agendamento(size_t id);
Paciente *ler_paciente(size_t id);
Medico *ler_medico(size_t id);
RelatorioAtendimento *ler_relatorio(size_t id);

size_t fappend(const void *content, size_t size, size_t amount, FILE *file);

#endif