#include "b_tree_disc.h"
#include "entidades_utils.h"
#include "file_manager.h"
#include "utils.h"

#define ITEM_POR_PAGINA 20

void listar_pacientes();
void listar_agendamentos();

int main(int argc, char *argv[]) {
    if (tem_argumento(argc, argv, "-h") || tem_argumento(argc, argv, "--help") || argc < 2) {
        printf("Uso: %s <-a | -p>\n", argv[0]);
        printf("  -a: Lista todos os agendamentos\n");
        printf("  -p: Lista todos os pacientes\n");
        return 0;
    }

    // Verifica o tipo de entidade
    if (strcmp(argv[1], "-a") == 0) {
        listar_agendamentos();
    } else if (strcmp(argv[1], "-p") == 0) {
        listar_pacientes();
    } else {
        print(LOG_ERROR, "Tipo de entidade inválido: %s\n", argv[1]);
        return 1;
    }
}

void listar_pacientes() {
    FILE *arquivo = obter_arquivo_dados(PACIENTE, false);

    if (arquivo == NULL) {
        print(LOG_ERROR, "Erro ao abrir o arquivo de pacientes\n");
        return;
    }

    char continuar = 's';
    Paciente pacientes[ITEM_POR_PAGINA];

    while (continuar == 's' && !feof(arquivo)) {
        int lidos = fread(pacientes, sizeof(Paciente), ITEM_POR_PAGINA, arquivo);

        printf("ID | Nome completo | Gênero | Data de nascimento | Hipertensão | Diabetes | "
               "Alcoolismo\n");
        for (int i = 0; i < lidos; i++) {
            imprimir_linha(PACIENTE, &pacientes[i]);
        }

        if (lidos < ITEM_POR_PAGINA) {
            break;
        } else {
            do {
                printf("Deseja exibir mais %d registros? (s/n): ", ITEM_POR_PAGINA);
                scanf(" %c", &continuar);
            } while (continuar != 's' && continuar != 'n');
        }
    }
}

void listar_agendamentos() {
    FILE *arquivo = obter_arquivo_dados(AGENDAMENTO, false);

    if (arquivo == NULL) {
        print(LOG_ERROR, "Erro ao abrir o arquivo de agendamentos\n");
        return;
    }

    char continuar = 's';
    Agendamento agendamentos[ITEM_POR_PAGINA];

    while (continuar == 's' && !feof(arquivo)) {
        int lidos = fread(agendamentos, sizeof(Agendamento), ITEM_POR_PAGINA, arquivo);

        printf("ID | ID Paciente | ID Médico | Data de agendamento | Data da consulta | Paciente "
               "compareceu | ID relatório\n");
        for (int i = 0; i < lidos; i++) {
            imprimir_linha(AGENDAMENTO, &agendamentos[i]);
        }

        if (lidos < ITEM_POR_PAGINA) {
            break;
        } else {
            do {
                printf("Deseja exibir mais %d registros? (s/n): ", ITEM_POR_PAGINA);
                scanf(" %c", &continuar);
            } while (continuar != 's' && continuar != 'n');
        }
    }
}