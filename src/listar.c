#include "b_tree_disc.h"
#include "entidades.h"
#include "entidades_utils.h"
#include "file_manager.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 10

/**
 * Atualiza um registro no arquivo de dados.
 *
 * Uso: ./buscar <-a | -p> <id>
 */
int main(int argc, char const *argv[]) {

    if (argc < 3) {
        print(LOG_ERROR, "Uso: %s <-a | -p | -m | -r>\n", argv[0]);
        return 1;
    }

    // Verifica o tipo de entidade
    TipoEntidade tipo;
    if (strcmp(argv[1], "-a") == 0) {
        tipo = AGENDAMENTO;
    } else if (strcmp(argv[1], "-p") == 0) {
        tipo = PACIENTE;
    } else if (strcmp(argv[1], "-m") == 0) {
        tipo = MEDICO;
    } else if (strcmp(argv[1], "-r") == 0) {
        tipo = RELATORIO;
    } else {
        print(LOG_ERROR, "Tipo de entidade inválido: %s\n", argv[1]);
        return 1;
    }

    // Inicializa o file manager
    if (!iniciar_file_manager(false)) {
        print(LOG_ERROR, "Erro ao inicializar o file manager\n");
        return 1;
    }

    // Carrega a árvore B do arquivo de índices
    bTree *arvore = carregar_arvore(obter_arquivo_indices(tipo));

    char continuar = 's';
    nodoBTree *nodo = arvore->raiz;

    while(continuar == 's'){
        int i = 0;
        int j = 0;
        while(i < PAGE_SIZE){
            if(nodo == NULL)
                continue;

                // TODO: terminar


            i++;
        }

        printf("Deseja exibir a próxima página? (s/n)");
        continuar = getchar();
        fflush(stdin);

    }
    return 0;
}
