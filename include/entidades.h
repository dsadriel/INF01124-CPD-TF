#ifndef entidades_h
#define entidades_h

#include <stdbool.h>
#include <stddef.h>

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
    size_t id;
    size_t id_paciente; // Chave relacionada
    size_t id_medico; // Chave estrangeira
    size_t id_relatorio; // Chave estrangeira
    Timestamp data_agendamento;  // Data e hora do agendamento
    Timestamp data_consulta; // Data e hora da consulta
    bool paciente_compareceu; // Se o paciente compareceu
} Agendamento;

/**
 * Estrutura de dados que representa um paciente
 */
typedef struct {
    size_t id;
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
    size_t id; 
    char nome[50]; // Nome do médico
    char sobrenome[50]; // Sobrenome do médico
    char especializacao[50]; // Especialização do médico
} Medico;

/**
 * Estrutura de dados que representa um relatório de atendimento
 */
typedef struct {
    size_t id;
    Timestamp data_inicio; // Data e hora de início do atendimento
    Timestamp data_final; // Data e hora de finalização do atendimento
    char resumo[100]; // Resumo do atendimento
} RelatorioAtendimento;



#endif