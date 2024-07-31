#ifndef entidades_h
#define entidades_h

#include <stdbool.h>

typedef enum {
    PACIENTE,
    MEDICO,
    AGENDAMENTO,
    RELATORIO
} TipoEntidade;

typedef struct {
    int dia;
    int mes;
    int ano;
    int hora;
    int minuto;
} Timestamp;


/**
 * Estrutura de dados que representa um agendamento
 */
typedef struct {
    int id;
    int id_paciente; // Chave relacionada
    int id_medico; // Chave estrangeira
    int id_relatorio; // Chave estrangeira
    Timestamp data_agendamento;  // Data e hora do agendamento
    Timestamp data_consulta; // Data e hora da consulta
    bool paciente_compareceu; // Se o paciente compareceu
} Agendamento;

/**
 * Estrutura de dados que representa um paciente
 */
typedef struct {
    int id; 
    char nome[50]; // Nome do paciente
    char sobrenome[50]; // Sobrenome do paciente
    char genero; // Gênero do paciente (M ou F)
    Timestamp data_nascimento; // Data de nascimento do paciente
    bool hipertensao; // Se o paciente tem hipertensão
    bool diabetes; // Se o paciente tem diabetes
    bool alcoolismo; // Se o paciente é alcoólatra
} Paciente;

/**
* Estrutura de dados que representa um médico
*/
typedef struct {
    int id; 
    char nome[50]; // Nome do médico
    char sobrenome[50]; // Sobrenome do médico
    char especializacao[50]; // Especialização do médico
} Medico;

/**
 * Estrutura de dados que representa um relatório de atendimento
 */
typedef struct {
    int id;
    Timestamp data_inicio; // Data e hora de início do atendimento
    Timestamp data_final; // Data e hora de finalização do atendimento
    char resumo[100]; // Resumo do atendimento
} RelatorioAtendimento;



#endif