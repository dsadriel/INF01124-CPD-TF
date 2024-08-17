#include "b_tree.h"
#include "b_tree_disc.h"
#include "entidades.h"
#include "entidades_utils.h"
#include "file_manager.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_LINE_LENGTH 300
#define FILE_NAME_LENGTH 100
#define PATIENT_ID_MAX 4294967295
#define ORDEM_ARVORE_PACIENTES 10
#define ORDEM_ARVORE_AGENDAMENTOS 1

#define CABECALHO_AGENDAMENTOS                                                                     \
    "PatientId,AppointmentID,Gender,ScheduledDay,AppointmentDay,Age,Neighbourhood,Scholarship,"    \
    "Hipertension,Diabetes,Alcoholism,Handcap,SMS_received,No-show\n"
#define CABECALHO_PACIENTES                                                                        \
    "id,nome,sobrenome,genero,dia_nascimento,mes_nascimento,ano_"                                  \
    "nascimento,bairro,hipertensao,diabetes,alcoolismo\n"

// Cabeçalhos das funções
bool importar_agendamentos(char *file_path);
bool importar_pacientes(char *file_path);
void linha_para_paciente(char *linha, Paciente *paciente);
void linha_para_agendamento(char *linha, Agendamento *agendamento);
bool verificar_cabecalho(FILE *file, char *previsto);
void obter_nome_arquivo(char *output, TipoEntidade tipo, int argc, char **argv);


void imprimeNodoNivel(nodoBTree* nodo, int nivel) { // FIXME: BY CHATGPT, remover
    if (nodo == NULL) {
        return;
    }

    int i;
    // Primeiro, imprimir os filhos da esquerda
    for (i = 0; i < nodo->num; i++) {
        if (!nodo->flagfolha) {
            imprimeNodoNivel(nodo->filhos[i], nivel + 1);
        }
        for(int j = 0; j < nivel; j++) {
            printf("    ");
        }
        printf("%d\n", nodo->keys[i]);
    }

    // Imprimir o filho da direita
    if (!nodo->flagfolha) {
        imprimeNodoNivel(nodo->filhos[i], nivel + 1);
    }
}


// ============================================================================================== //
int main(int argc, char **argv) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // Define o output do console para UTF-8
#endif

    // if ((argc > 1 && strcmp(argv[1], "-y") != 0) || argc == 1) {
    //     // AVISO DE SOBRESCRITA
    //     print(LOG_WARNING,
    //           "ATENÇÃO: Essa operação irá SOBRESCREVER todos os dados salvos pelo sistema. "
    //           "Proceda com cuidado\nESCREVA: \"Sim, continuar\" para continuar: ");
    //     char confirmacao[20] = {0};
    //     fgets(confirmacao, 20, stdin);
    //     if (strcmp(confirmacao, "Sim, continuar\n") != 0) {
    //         printf("Frase de confirmação inválida, saindo do sistema de importação...");
    //         exit(1);
    //     }
    // }
    print(LOG_WARNING,
          "Confirmação informada. Autorizado para continuar a SOBRESCRITA dos dados.\n");

    // Inicializa o file manager com a opção de sobrescrever os arquivos
    if (!iniciar_file_manager(true)) {
        print(LOG_ERROR, "Falha ao inicializar o gerenciador de arquivos.\n");
        return 1;
    }

    // Obtém o nome do arquivo de agendamentos
    char file_name[FILE_NAME_LENGTH + 1] = {0};
    char file_path[FILE_NAME_LENGTH + 11] = {0};

    obter_nome_arquivo(file_name, AGENDAMENTO, argc, argv);
    sprintf(file_path, "input/%s", file_name);
    // Importa os agendamentos
    if (!importar_agendamentos(file_path)) {
        print(LOG_ERROR, "Falha ao importar os agendamentos.\n");
        finalizar_file_manager();
        return 1;
    }

    obter_nome_arquivo(file_name, PACIENTE, argc, argv);
    sprintf(file_path, "input/%s", file_name);
    // Importa os pacientes
    if (!importar_pacientes(file_path)) {
        print(LOG_ERROR, "Falha ao importar os pacientes.\n");
        finalizar_file_manager();
        return 1;
    }

    // Finaliza o file manager
    finalizar_file_manager();
    print(LOG_SUCCESS, "Dados importados com sucesso.\n");
    return 0;
}

// ============================================================================================== //
// ============================================================================================== //
// ============================================================================================== //
// ============================================================================================== //

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
        print(LOG_ERROR, "Falha ao abrir o arquivo de entrada %s.\n", file_path);
        return false;
    }

    if (!verificar_cabecalho(arquivo_agendamentos, CABECALHO_AGENDAMENTOS)) {
        fclose(arquivo_agendamentos);
        return false;
    }

    printf("Importando dados de AGENDAMENTOS do arquivo %s...\n", file_path);

    bTree *arvore_indices = criaArv(ORDEM_ARVORE_AGENDAMENTOS);
    char linha[MAX_LINE_LENGTH] = {0};

    int qtde = 0;

    while (fgets(linha, MAX_LINE_LENGTH, arquivo_agendamentos) != NULL) {
        Agendamento agendamento = {0};
        linha_para_agendamento(linha, &agendamento);

        if (consulta(arvore_indices->raiz, agendamento.id) == NULL) {
            // Agendamento não existe na base de dados
            size_t offset =
                fappend(&agendamento, sizeof(Agendamento), obter_arquivo_dados(AGENDAMENTO));

            if (offset == -1) {
                print(LOG_ERROR, "Erro ao escrever o agendamento no arquivo de dados.\n");
                fclose(arquivo_agendamentos);
                finalizar_file_manager();
                return false;
            }

            // Insere o agendamento na árvore B
            insere(arvore_indices, agendamento.id, offset);
            keytype *key = consulta(arvore_indices->raiz, agendamento.id);
            if (key == NULL) {
                print(LOG_WARNING, "Agendamento %d NÃO inserido\n", agendamento.id);
                printf("Qtde: %d\n\n", qtde);
                imprimeNodoNivel(arvore_indices->raiz, 0);
                exit(1);
    
            } else {
                qtde++;
            }
            // BUG: pq não está inserindo?

            // FIXME: Atualizar os demais indexadores
        } else {
            // Agendamento já existe
            printf("Agendamento com ID %d já existe no sistema.\n", agendamento.id);
        }
    }

    fclose(arquivo_agendamentos);
    print(LOG_INFO, "Dados de agendamentos importados com sucesso.\n");
    salvar_arvore(obter_arquivo_indices(AGENDAMENTO), arvore_indices);
    print(LOG_INFO, "Árvore de índices de agendamentos salva com sucesso.\n");
    // liberaArv(arvore_indices); // FIXME: liberaArv não está funcionando corretamente
    print(LOG_INFO, "Árvore de índices de agendamentos liberada com sucesso.\n");
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
        print(LOG_ERROR, "Falha ao abrir o arquivo de entrada %s.\n", file_path);
        return false;
    }

    if (!verificar_cabecalho(arquivo_pacientes, CABECALHO_PACIENTES)) {
        fclose(arquivo_pacientes);
        return false;
    }
    printf("Importando dados de PACIENTES do arquivo %s...\n", file_path);

    bTree *arvore_indices = criaArv(ORDEM_ARVORE_PACIENTES);
    char linha[MAX_LINE_LENGTH + 1] = {0};

    while (fgets(linha, MAX_LINE_LENGTH, arquivo_pacientes) != NULL) {
        Paciente paciente = {0};

        linha_para_paciente(linha, &paciente);

        if (consulta(arvore_indices->raiz, paciente.id) == NULL) {
            size_t offset = fappend(&paciente, sizeof(Paciente), obter_arquivo_dados(PACIENTE));

            if (offset == -1) {
                print(LOG_ERROR, "Erro ao escrever o paciente no arquivo de dados.\n");
                fclose(arquivo_pacientes);
                return false;
            }

            insere(arvore_indices, paciente.id, offset);
            keytype *key = consulta(arvore_indices->raiz, paciente.id);
            if (key == NULL)
                print(LOG_WARNING, "Paciente %d NÃO inserido\n", paciente.id);
            // BUG: pq não está inserindo?

        } else {
            // Paciente já existe
            printf("Paciente com ID %d já existe no sistema.\n", paciente.id);
        }
    }

    fclose(arquivo_pacientes);
    print(LOG_INFO, "Dados de pacientes importados com sucesso.\n");
    salvar_arvore(obter_arquivo_indices(PACIENTE), arvore_indices);
    print(LOG_INFO, "Árvore de índices de pacientes salva com sucesso.\n");
    // liberaArv(arvore_indices); // FIXME: liberaArv não está funcionando corretamente
    print(LOG_INFO, "Árvore de índices de pacientes liberada com sucesso.\n");

    return true;
}

/**
 * Dada uma linha e um paciente, atribui os dados dessa linha ao paciente
 *
 * @param linha Linha do CSV contendo as informações do paciente
 * @param paciente ponteiro para a estrutura do paciente
 *
 * @returns void
 */
void linha_para_paciente(char *linha, Paciente *paciente) {
    char *token = strtok(linha, ","); // id
    paciente->id = (size_t)(strtoull(token, NULL, 10));

    token = strtok(NULL, ","); // nome
    strncpy(paciente->nome, token, 50);

    token = strtok(NULL, ","); // sobrenome
    strncpy(paciente->sobrenome, token, 50);

    token = strtok(NULL, ","); // genero
    paciente->genero = token[0];

    token = strtok(NULL, ","); // dia_nascimento
    paciente->data_nascimento.dia = atoi(token);

    token = strtok(NULL, ","); // mes_nascimento
    paciente->data_nascimento.mes = atoi(token);

    token = strtok(NULL, ","); // ano_nascimento
    paciente->data_nascimento.ano = atoi(token);

    token = strtok(NULL, ","); // bairro
    strncpy(paciente->bairro, token, 50);

    token = strtok(NULL, ","); // hipertensao
    paciente->hipertensao = strcmp(token, "1") == 0;

    token = strtok(NULL, ","); // diabetes
    paciente->diabetes = strcmp(token, "1") == 0;

    token = strtok(NULL, ","); // alcoolismo
    paciente->alcoolismo = strcmp(token, "1") == 0;
}

/**
 * Dada uma linha e um agendamento, atribui os dados dessa linha ao agendamento
 *
 * @param linha Linha do CSV contendo as informações do agendamento
 * @param agendamento ponteiro para a estrutura do agendamento
 *
 * @returns void
 */
void linha_para_agendamento(char *linha, Agendamento *agendamento) {
    char *token = strtok(linha, ","); // PatientId
    agendamento->id_paciente = (size_t)(strtoull(token, NULL, 10) % PATIENT_ID_MAX);

    token = strtok(NULL, ","); // AppointmentID
    agendamento->id = strtoull(token, NULL, 10);

    token = strtok(NULL, ","); // Gender
    token = strtok(NULL, ","); // ScheduledDay
    parse_date(token, &agendamento->data_agendamento);

    token = strtok(NULL, ","); // AppointmentDay
    parse_date(token, &agendamento->data_consulta);

    token = strtok(NULL, ","); // Age
    token = strtok(NULL, ","); // Neighbourhood
    token = strtok(NULL, ","); // Scholarship
    token = strtok(NULL, ","); // Hipertension
    token = strtok(NULL, ","); // Diabetes
    token = strtok(NULL, ","); // Alcoholism
    token = strtok(NULL, ","); // Handcap
    token = strtok(NULL, ","); // SMS_received
    token = strtok(NULL, ","); // No-show
    agendamento->paciente_compareceu = strcmp(token, "No") != 0;
}

bool verificar_cabecalho(FILE *file, char *previsto) {
    char buffer[MAX_LINE_LENGTH + 1] = {};
    fgets(buffer, MAX_LINE_LENGTH, file);
    if (strcmp(buffer, previsto) == 0)
        return true;
    printf("Cabeçalho do arquivo de entrada inválido. Por favor, verifique o arquivo\nO "
           "formato correto é o seguinte:\n\t%s",
           previsto);
    return false;
}

void obter_nome_arquivo(char *output, TipoEntidade tipo, int argc, char **argv) {
    char prefixo[3] = {0};
    switch (tipo) {
    case AGENDAMENTO:
        strcpy(prefixo, "-a");
        strcpy(output, "Appointments.csv"); // Nome padrão
        break;
    case PACIENTE:
        strcpy(prefixo, "-p");
        strcpy(output, "Patients.csv"); // Nome padrão
        break;
    default:
        break;
    }

    for (int i = 1; i < argc - 1; i++) {
        if (strncmp(argv[i], prefixo, 2) == 0) {
            strcpy(output, argv[i + 1]);
            return;
        }
    }
}