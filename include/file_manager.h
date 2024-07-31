#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdio.h>
#include <entidades.h>

#define NUM_ARQUIVOS 4

// Inicialização do file manager
bool iniciar_file_manager();
void finalizar_file_manager();

// Obtenção de índices através do indexador
size_t obter_indice_arquivo(size_t id, TipoEntidade tipo);

// Agendamentos 
Agendamento *ler_agendamento(size_t id);

// Pacientes
Paciente *ler_paciente(size_t id);

// Medicos
Medico *ler_medico(size_t id);

// Relatorios
RelatorioAtendimento *ler_relatorio(size_t id);

#endif