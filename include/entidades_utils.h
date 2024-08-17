#ifndef ENTIDADES_UTILS_H
#define ENTIDADES_UTILS_H


#include <stdio.h>
#include "entidades.h"

void parse_date(char *date, Timestamp *tm);
void imprimir(TipoEntidade tipo, void* entidade);
void __imprimir_paciente(Paciente* paciente);
void __imprimir_medico(Medico* medico);
void __imprimir_agendamento(Agendamento* agendamento);
Timestamp obter_data_hora_atual();

void ler_entidade(TipoEntidade tipo, void *entidade);
void __ler_paciente(Paciente *paciente);
void __ler_agendamento(Agendamento *agendamento);

#endif // ENTIDADES_UTILS_H