#include "arquivo_invertido.h"
#include "b_tree_disc.h"
#include "entidades.h"
#include "entidades_utils.h"
#include "file_manager.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 10

int main(int argc, char const *argv[]) {
    printf("Não implementado\n");
    exit(-1);

    if (argc < 2 || strcmp(argv[1], "-d") != 0) {
        print(LOG_ERROR, "Uso: %s -d DD/MM/AAAA\n", argv[0]);
        return 1;
    }

    // Inicializa o file manager
    if (!iniciar_file_manager(false)) {
        print(LOG_ERROR, "Erro ao inicializar o file manager\n");
        return 1;
    }

    // Define a data a ser buscada
    int dia, mes, ano;
    sscanf(argv[2], "%2d/%2d/%4d", &dia, &mes, &ano);

    int data = (ano * 10000) + (mes * 100) + dia;
    if (data < 0 || data > 99999999) {
        print(LOG_ERROR, "Data inválida\n");
        return 1;
    }

    printf("Data: %d\n", data);

    // Carrega a árvore B do arquivo de índices
    bTree *arvore_pacientes = carregar_arvore(obter_arquivo_indices(PACIENTE));

    // Carrega o arquivo invertido
    ArquivoInvertido *arquivo = carregarArquivoInvertido("./data/agendamentos_data.inverted");

    // Busca a data no arquivo invertido
    Posting *posting = consultaArquivoInvertido(arquivo, data);

    char continuar = 's';
    int ultimo_exibido = 0;
    while (continuar == 's') {
        if (posting == NULL) {
            printf("Nenhum registro para exibir\n");
            return 1;
        }

        printf(" Agendamento | Paciente\n");
        while (ultimo_exibido % PAGE_SIZE != 0 && posting != NULL) {
            int i = ultimo_exibido++;

            if (ultimo_exibido >= arquivo->__num_indices_max) {
                ultimo_exibido = 0;
                if (posting->prox == NULL) {
                    printf("Fim dos registros\n");
                    return 0;
                } else {
                    Posting *aux = posting;
                    posting = posting->prox;
                    free(aux);
                }
            }

            Agendamento *ag = ler_agendamento_disco(posting->registros[i].offset);
            int offset_paciente = consulta(arvore_pacientes->raiz, ag->id_paciente)->offset;
            if (offset_paciente == -1) {
                printf("Paciente não encontrado\n");
                return 1;
            }
            Paciente *pac = ler_paciente_disco(offset_paciente);

            printf(" %11d | %s\n", ag->id, pac->nome);
        }

        printf("Deseja exibir a próxima página? (s/n)");
        continuar = getchar();
        fflush(stdin);
        Posting *aux = posting;
        posting = posting->prox;
        free(aux);
    }
    return 0;
}
