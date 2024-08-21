
#include "entidades_utils.h"
#include <time.h>
#include <string.h>

void parse_date(char *date, Timestamp *tm) {
    sscanf(date, "%hd-%hd-%hdT%hd:%hd", &tm->ano, &tm->mes, &tm->dia, &tm->hora, &tm->minuto);
}

void imprimir(TipoEntidade tipo, void *entidade) {
    switch (tipo) {
    case PACIENTE:
        __imprimir_paciente((Paciente *)entidade);
        break;
    
    case AGENDAMENTO:
        __imprimir_agendamento((Agendamento *)entidade);
        break;
    default:
        break;
    }
}

void imprimir_linha(TipoEntidade tipo, void *entidade) {
    switch (tipo) {
    case PACIENTE:
        __imprimir_paciente_linha((Paciente *)entidade);
        break;
    case AGENDAMENTO:
        __imprimir_agendamento_linha((Agendamento *)entidade);
        break;
    default:
        break;
    }
}

void __imprimir_paciente(Paciente *paciente) {
    printf("ID: %u\n", paciente->id);
    printf("Nome: %s %s\n", paciente->nome, paciente->sobrenome);
    printf("Gênero: %c\n", paciente->genero);
    printf("Data de nascimento: %02d/%02d/%04d\n", paciente->data_nascimento.dia,
           paciente->data_nascimento.mes, paciente->data_nascimento.ano);
    printf("Bairro: %s\n", paciente->bairro);
    printf("Hipertensão: %s\n", paciente->hipertensao ? "Sim" : "Não");
    printf("Diabetes: %s\n", paciente->diabetes ? "Sim" : "Não");
    printf("Alcoolismo: %s\n", paciente->alcoolismo ? "Sim" : "Não");
}

void __imprimir_paciente_linha(Paciente *paciente) {
    printf("%u | %s %s |   %c    | %02d/%02d/%04d |    %s   |    %s   |    %s   \n",
           paciente->id, paciente->nome, paciente->sobrenome, paciente->genero,
           paciente->data_nascimento.dia, paciente->data_nascimento.mes,
           paciente->data_nascimento.ano, paciente->hipertensao ? "Sim" : "Não",
           paciente->diabetes ? "Sim" : "Não", paciente->alcoolismo ? "Sim" : "Não");
}

void __imprimir_agendamento(Agendamento *agendamento) {
    printf("ID: %d\n", agendamento->id);
    printf("ID Paciente: %u\n", agendamento->id_paciente);
    printf("ID Médico: %u\n", agendamento->id_medico);
    printf("Data de agendamento: %02d/%02d/%04d %02d:%02d\n", agendamento->data_agendamento.dia,
           agendamento->data_agendamento.mes, agendamento->data_agendamento.ano,
           agendamento->data_agendamento.hora, agendamento->data_agendamento.minuto);
    printf("Data da consulta: %02d/%02d/%04d %02d:%02d\n", agendamento->data_consulta.dia,
           agendamento->data_consulta.mes, agendamento->data_consulta.ano,
           agendamento->data_consulta.hora, agendamento->data_consulta.minuto);
    printf("Paciente compareceu: %s\n", agendamento->paciente_compareceu ? "Sim" : "Não");

    if (agendamento->paciente_compareceu) {
        printf("ID Relatório: %u\n", agendamento->id_relatorio);
    }
}

void __imprimir_agendamento_linha(Agendamento *agendamento) {
    printf("%10u | %10u | %10u | %02d/%02d/%04d %02d:%02d | %02d/%02d/%04d %02d:%02d |    %s   | %10u\n",
           agendamento->id, agendamento->id_paciente, agendamento->id_medico,
           agendamento->data_agendamento.dia, agendamento->data_agendamento.mes,
           agendamento->data_agendamento.ano, agendamento->data_agendamento.hora,
           agendamento->data_agendamento.minuto, agendamento->data_consulta.dia,
           agendamento->data_consulta.mes, agendamento->data_consulta.ano,
           agendamento->data_consulta.hora, agendamento->data_consulta.minuto,
           agendamento->paciente_compareceu ? "Sim" : "Não", agendamento->id_relatorio);
}

Timestamp obter_data_hora_atual() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    Timestamp timestamp;
    timestamp.ano = tm.tm_year + 1900; // Ano atual
    timestamp.mes = tm.tm_mon + 1;     // Mês atual
    timestamp.dia = tm.tm_mday;        // Dia atual
    timestamp.hora = tm.tm_hour;       // Hora atual
    timestamp.minuto = tm.tm_min;      // Minuto atual

    return timestamp;
}

void ler_entidade(TipoEntidade tipo, void *entidade) {
    switch (tipo) {
    case PACIENTE:
        __ler_paciente((Paciente *)entidade);
        break;
    case AGENDAMENTO:
        __ler_agendamento((Agendamento *)entidade);
        break;
    default:
        break;
    }
}

void __ler_paciente(Paciente *paciente) {
    printf("Nome: ");
    fgets(paciente->nome, 50, stdin);
    paciente->nome[strcspn(paciente->nome, "\n")] = 0;

    printf("Sobrenome: ");
    fgets(paciente->sobrenome, 50, stdin);
    paciente->sobrenome[strcspn(paciente->sobrenome, "\n")] = 0;

    printf("Gênero (M ou F): ");
    scanf(" %c", &paciente->genero);

    printf("Data de nascimento (dd/mm/aaaa): ");
    scanf("%hd/%hd/%hd", &paciente->data_nascimento.dia, &paciente->data_nascimento.mes,
          &paciente->data_nascimento.ano);

    printf("Hipertensão (1 - sim, 0 - não): ");
    int hipertensao;
    scanf("%d", &hipertensao);
    paciente->hipertensao = (bool)hipertensao;

    printf("Diabetes (1 - sim, 0 - não): ");
    int diabetes;
    scanf("%d", &diabetes);
    paciente->diabetes = (bool)diabetes;

    printf("Alcoolismo (1 - sim, 0 - não): ");
    int alcoolismo;
    scanf("%d", &alcoolismo);
    paciente->alcoolismo = (bool)alcoolismo;
}

void __ler_agendamento(Agendamento *agendamento) {
    printf("ID Paciente: ");
    scanf("%u", &agendamento->id_paciente);

    printf("ID Médico: ");
    scanf("%u", &agendamento->id_medico);

    printf("ID Relatório: ");
    scanf("%u", &agendamento->id_relatorio);

    printf("Data de agendamento (dd/mm/aaaa hh:mm): ");
    scanf("%hd/%hd/%hd %hd:%hd", &agendamento->data_agendamento.dia, &agendamento->data_agendamento.mes,
          &agendamento->data_agendamento.ano, &agendamento->data_agendamento.hora,
          &agendamento->data_agendamento.minuto);

    printf("Data da consulta (dd/mm/aaaa hh:mm): ");
    scanf("%hd/%hd/%hd %hd:%hd", &agendamento->data_consulta.dia, &agendamento->data_consulta.mes,
          &agendamento->data_consulta.ano, &agendamento->data_consulta.hora,
          &agendamento->data_consulta.minuto);

    printf("Paciente compareceu (1 - sim, 0 - não): ");
    int paciente_compareceu;
    scanf("%d", &paciente_compareceu);
    agendamento->paciente_compareceu = (bool)paciente_compareceu;
}