#include "entidades.h"
#include "entidades_utils.h"
#include "file_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_LINE_LENGTH 300
#define FILE_NAME_LENGTH 100

bool importar_agendamentos(char *file_path);
bool importar_pacientes(char *file_path);

int main(int argc, char **argv) {
    SetConsoleOutputCP(CP_UTF8); // Define o output do console para UTF-8

    // Inicializa o file manager
    if (iniciar_file_manager()) {
        printf("Serviço de importação de dados inicializado com sucesso.\n");
    } else {
        printf("Falha ao inicializar o serviço de importação de dados.\n");
        return 1;
    }

    // Obtem o nome do arquivo de agendamentos
    char file_name[FILE_NAME_LENGTH] = {0};
    if (argc == 2) {
        strncpy(file_name, argv[1],
                FILE_NAME_LENGTH); // Copia o nome do arquivo passado como argumento
    } else {
        strncpy(file_name, "Appointments.csv", FILE_NAME_LENGTH); // Nome padrão do arquivo
    }
    char file_path[FILE_NAME_LENGTH + 10];
    sprintf(file_path, "input/%s", file_name);

    // Importa os agendamentos
    if (!importar_agendamentos(file_path)) {
        printf("Falha ao importar os agendamentos.\n");
        finalizar_file_manager();
        return 1;
    }

    // Obtem o nome do arquivo de pacientes
    if (argc == 3) {
        strncpy(file_name, argv[2],
                FILE_NAME_LENGTH); // Copia o nome do arquivo passado como argumento
    } else {
        strncpy(file_name, "Patients.csv", FILE_NAME_LENGTH); // Nome padrão do arquivo
    }
    sprintf(file_path, "input/%s", file_name);

    // Importa os pacientes
    if (!importar_pacientes(file_path)) {
        printf("Falha ao importar os pacientes.\n");
        finalizar_file_manager();
        return 1;
    }

    finalizar_file_manager();
    
    printf("Dados importados com sucesso.\n");
    return 0;
}

/**
 * Importa os agendamentos de um arquivo CSV para a base de dados
 * 
 * @param file_path Caminho do arquivo CSV
 * 
 * @return true se a importação foi bem sucedida, false caso contrário
 */
bool importar_agendamentos(char *file_path) {
    // Abre o arquivo de agendamentos para leitura
    FILE *arquivo_agendamentos = fopen(file_path, "r");
    if (arquivo_agendamentos == NULL) {
        printf("Falha ao abrir o arquivo de entrada.\n");
        finalizar_file_manager();
        return false;
    }

    printf("Importando dados do arquivo %s...\n", file_path);

    char linha[MAX_LINE_LENGTH] = {0};
    fgets(linha, MAX_LINE_LENGTH, arquivo_agendamentos);
    if (strcmp(linha, "PatientId,AppointmentID,Gender,ScheduledDay,AppointmentDay,Age,"
                      "Neighbourhood,Scholarship,Hipertension,Diabetes,Alcoholism,Handcap,"
                      "SMS_received,No-show\n") != 0) {
        printf("Cabeçalho do arquivo de entrada inválido. Por favor, verifique o arquivo\n");
        fclose(arquivo_agendamentos);
        finalizar_file_manager();
        return 1;
    }

    while (fgets(linha, MAX_LINE_LENGTH, arquivo_agendamentos) != NULL) {
        Agendamento agendamento = {0};

        char *token = strtok(linha, ","); // PatientId
        agendamento.id_paciente = strtoull(token, NULL, 10);

        token = strtok(NULL, ","); // AppointmentID
        agendamento.id = strtoull(token, NULL, 10);

        token = strtok(NULL, ","); // Gender
        token = strtok(NULL, ","); // ScheduledDay
        parse_date(token, &agendamento.data_agendamento);

        token = strtok(NULL, ","); // AppointmentDay
        parse_date(token, &agendamento.data_consulta);

        token = strtok(NULL, ","); // Age
        token = strtok(NULL, ","); // Neighbourhood
        token = strtok(NULL, ","); // Scholarship
        token = strtok(NULL, ","); // Hipertension
        token = strtok(NULL, ","); // Diabetes
        token = strtok(NULL, ","); // Alcoholism
        token = strtok(NULL, ","); // Handcap
        token = strtok(NULL, ","); // SMS_received
        token = strtok(NULL, ","); // No-show
        agendamento.paciente_compareceu = strcmp(token, "No") != 0;

        int indice = obter_indice_arquivo(agendamento.id, AGENDAMENTO);
        if (indice == -1) {
            // Agendamento não existe na base de dados
            if (fappend(&agendamento, sizeof(Agendamento), 1, obter_arquivo_dados(AGENDAMENTO)) !=
                1) {
                printf("Erro ao escrever o agendamento no arquivo de dados.\n");
                fclose(arquivo_agendamentos);
                finalizar_file_manager();
                return false;
            }
        } else {
            // Agendamento já existe
            printf("Agendamento com ID %d já existe no sistema.\n", agendamento.id);
            // TODO: implementar política de resolução de conflitos
            // TODO: Adicionar lógica para inserir o agendamento no índice
        }

        break; // Remova essa linha após implementar a lógica de inserção no índice
    }

    fclose(arquivo_agendamentos);
    return true;
}

/**
 * Importa os pacientes de um arquivo CSV para a base de dados
 * 
 * @param file_path Caminho do arquivo CSV
 * 
 * @return true se a importação foi bem sucedida, false caso contrário
 */
bool importar_pacientes(char *file_path) {
    FILE *arquivo_pacientes = fopen(file_path, "r");
    if (arquivo_pacientes == NULL) {
        printf("Falha ao abrir o arquivo de entrada.\n");
        finalizar_file_manager();
        return false;
    }

    printf("Importando dados do arquivo %s...\n", file_path);

    char linha[MAX_LINE_LENGTH] = {0};
    fgets(linha, MAX_LINE_LENGTH, arquivo_pacientes);
    if (strcmp(linha, "id,nome,sobrenome,genero,dia_nascimento,mes_nascimento,ano_nascimento,"
                      "bairro,hipertensao,diabetes,alcoolismo\n") != 0) {
        printf("Cabeçalho do arquivo de entrada inválido. Por favor, verifique o arquivo\n");
        fclose(arquivo_pacientes);
        finalizar_file_manager();
        return 1;
    }

    while (fgets(linha, MAX_LINE_LENGTH, arquivo_pacientes) != NULL) {
        Paciente paciente = {0};

        char *token = strtok(linha, ","); // id
        paciente.id = strtoull(token, NULL, 10);

        token = strtok(NULL, ","); // nome
        strncpy(paciente.nome, token, 50);

        token = strtok(NULL, ","); // sobrenome
        strncpy(paciente.sobrenome, token, 50);

        token = strtok(NULL, ","); // genero
        paciente.genero = token[0];

        token = strtok(NULL, ","); // dia_nascimento
        paciente.data_nascimento.dia = atoi(token);

        token = strtok(NULL, ","); // mes_nascimento
        paciente.data_nascimento.mes = atoi(token);

        token = strtok(NULL, ","); // ano_nascimento
        paciente.data_nascimento.ano = atoi(token);

        token = strtok(NULL, ","); // bairro
        // FIXME: vamos usar isso para algo?

        token = strtok(NULL, ","); // hipertensao
        paciente.hipertensao = strcmp(token, "1") == 0;

        token = strtok(NULL, ","); // diabetes
        paciente.diabetes = strcmp(token, "1") == 0;

        token = strtok(NULL, ","); // alcoolismo
        paciente.alcoolismo = strcmp(token, "1") == 0;

        int indice = obter_indice_arquivo(paciente.id, PACIENTE);
        if (indice == -1) {
            // Paciente não existe na base de dados
            if (fappend(&paciente, sizeof(Paciente), 1, obter_arquivo_dados(PACIENTE)) != 1) {
                printf("Erro ao escrever o paciente no arquivo de dados.\n");
                fclose(arquivo_pacientes);
                finalizar_file_manager();
                return false;
            }
        } else {
            // Paciente já existe
            printf("Paciente com ID %d já existe no sistema.\n", paciente.id);
            // TODO: implementar política de resolução de conflitos
        }

        break; // Remova essa linha após implementar a lógica de inserção no índice
    }

    Paciente *paciente = ler_paciente(0);
    imprimir(PACIENTE, paciente);

    fclose(arquivo_pacientes);
    return true;
}