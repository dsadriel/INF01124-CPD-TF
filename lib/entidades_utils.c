
#include "entidades_utils.h"

void parse_date(char *date, Timestamp *tm) {
    sscanf(date, "%d/%d/%d %d:%d", &tm->dia, &tm->mes, &tm->ano, &tm->hora, &tm->minuto);
}

void imprimir(TipoEntidade tipo, void *entidade) {
    switch (tipo) {
    case PACIENTE:
        __imprimir_paciente((Paciente *)entidade);
        break;
    case MEDICO:
        __imprimir_medico((Medico *)entidade);
        break;
    case AGENDAMENTO:
        __imprimir_agendamento((Agendamento *)entidade);
        break;
    default:
        break;
    }
}

void __imprimir_paciente(Paciente *paciente) {
    printf("ID: %llu\n", paciente->id);
    printf("Nome: %s %s\n", paciente->nome, paciente->sobrenome);
    printf("Gênero: %c\n", paciente->genero);
    printf("Data de nascimento: %02d/%02d/%04d\n", paciente->data_nascimento.dia,
           paciente->data_nascimento.mes, paciente->data_nascimento.ano);
    printf("Hipertensão: %s\n", paciente->hipertensao ? "Sim" : "Não");
    printf("Diabetes: %s\n", paciente->diabetes ? "Sim" : "Não");
    printf("Alcoolismo: %s\n", paciente->alcoolismo ? "Sim" : "Não");
}

void __imprimir_medico(Medico *medico) {
    printf("ID: %zu\n", medico->id);
    printf("Nome: %s %s\n", medico->nome, medico->sobrenome);
    printf("Especialização: %s\n", medico->especializacao);
}

void __imprimir_agendamento(Agendamento *agendamento) {
    printf("ID: %d\n", agendamento->id);
    printf("ID Paciente: %llu\n", agendamento->id_paciente);
    printf("ID Médico: %zu\n", agendamento->id_medico);
    printf("ID Relatório: %zu\n", agendamento->id_relatorio);
    printf("Data de agendamento: %02d/%02d/%04d %02d:%02d\n", agendamento->data_agendamento.dia,
           agendamento->data_agendamento.mes, agendamento->data_agendamento.ano,
           agendamento->data_agendamento.hora, agendamento->data_agendamento.minuto);
    printf("Data da consulta: %02d/%02d/%04d %02d:%02d\n", agendamento->data_consulta.dia,
           agendamento->data_consulta.mes, agendamento->data_consulta.ano,
           agendamento->data_consulta.hora, agendamento->data_consulta.minuto);
    printf("Paciente compareceu: %s\n", agendamento->paciente_compareceu ? "Sim" : "Não");
}