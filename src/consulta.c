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

int main(int argc, char *argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // Define o output do console para UTF-8
#endif

    if(tem_argumento(argc, argv, "-h") || tem_argumento(argc, argv, "--help") || argc < 3) {
        printf("Uso: %s <-a | -p > <id>\n", argv[0]);
        printf("  -a: Consulta um agendamento\n");
        printf("  -p: Consulta um paciente\n");
        return 0;
    }


    // Verifica o tipo de entidade
    TipoEntidade tipo;
    if (strcmp(argv[1], "-a") == 0) {
        tipo = AGENDAMENTO;
    } else if (strcmp(argv[1], "-p") == 0) {
        tipo = PACIENTE;
    // } else if (strcmp(argv[1], "-m") == 0) {
    //     tipo = MEDICO;
    // } else if (strcmp(argv[1], "-r") == 0) {
    //     tipo = RELATORIO;
    } else {
        print(LOG_ERROR, "Tipo de entidade inválido: %s\n", argv[1]);
        return 1;
    }


    // Carrega a árvore B do arquivo de índices
    bTree *arvore = carregar_arvore(obter_arquivo_indices(tipo, false));
    if (arvore == NULL) {
        print(LOG_ERROR, "Erro ao carregar a árvore B do arquivo de índices\n");
        return 1;
    }

    // Busca o registro
    keytype *key = consulta(arvore->raiz, atoi(argv[2]));
    if (key == NULL) {
        print(LOG_ERROR, "Registro não encontrado\n");
        return 1;
    }
    int offset = key->offset;

    void *registro = NULL;

    // Solicita a atualização do registro
    switch (tipo) {
    case AGENDAMENTO:
        printf(YELLOW "AGENDAMENTO:\n" RESET);
        registro = ler_agendamento_disco(offset);
        imprimir(AGENDAMENTO, registro);

        liberaArv(arvore);
        arvore = carregar_arvore(obter_arquivo_indices(PACIENTE, false));
        key = consulta(arvore->raiz, ((Agendamento *)registro)->id_paciente);

        free(registro);
        if (key == NULL) {
            printf(RED "> Paciente não encontrado\n" RESET);
            break;
        }
        offset = key->offset;
    case PACIENTE:
        printf(YELLOW "PACIENTE:\n" RESET);
        registro = ler_paciente_disco(offset);
        imprimir(PACIENTE, registro);
        break;
    case MEDICO:
    case RELATORIO:
    default:
        break;
    }

    liberaArv(arvore);
    free(registro);
    return 0;
}
