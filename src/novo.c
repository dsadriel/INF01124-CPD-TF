#include "b_tree_disc.h"
#include "entidades.h"
#include "entidades_utils.h"
#include "file_manager.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

size_t novo_agendamento(bTree *arvore);
size_t novo_paciente(bTree *arvore);

int main(int argc, char const *argv[]) {

    if (argc < 2) {
        print(LOG_ERROR, "Uso: %s <-a | -p>\n", argv[0]);
        return 1;
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

    // Inicializa o file manager
    if (!iniciar_file_manager(false)) {
        print(LOG_ERROR, "Erro ao inicializar o file manager\n");
        return 1;
    }

    // Carrega a árvore B do arquivo de índices
    bTree *arvore = carregar_arvore(obter_arquivo_indices(tipo));
    if (arvore == NULL) {
        print(LOG_ERROR, "Erro ao carregar a árvore B do arquivo de índices\n");
        return 1;
    }

    size_t id = -1;
    switch (tipo) {
    case AGENDAMENTO:
        id = novo_agendamento(arvore);
        break;
    case PACIENTE:
        id = novo_paciente(arvore);
        break;
    default:
        break;
    }

    print(LOG_SUCCESS, "Registro criado com sucesso. ID: %u\n", id);

    return 0;
}


/**
 * Cria um novo agendamento com base nos dados fornecidos pelo usuário e o adiciona ao arquivo de dados
 * 
 * @param bTree *arvore de índices
 * 
 * @return ID do agendamento
 */
size_t novo_agendamento(bTree *arvore) {

    Agendamento ag = {0};

    ler_entidade(AGENDAMENTO, &ag);
    
    ag.id = clock() % UINT_MAX;
    // Verifica se o ID já existe na árvore
    while(consulta(arvore->raiz, ag.id) != NULL) {
        ag.id = clock() % UINT_MAX;
    }

    // Adiciona o registro ao arquivo de dados
    size_t offset = fappend(&ag, sizeof(Agendamento), obter_arquivo_dados(AGENDAMENTO));
    insere(arvore, ag.id, offset);
    salvar_arvore( obter_arquivo_indices(AGENDAMENTO), arvore);

    return ag.id;
}

/**
 * Cria um novo paciente com base nos dados fornecidos pelo usuário e o adiciona ao arquivo de dados
 * 
 * @param bTree *arvore de índices
 */

size_t novo_paciente(bTree *arvore) {

    Paciente paciente = {0};

    ler_entidade(PACIENTE, &paciente);

    paciente.id = clock() % UINT_MAX;
    // Verifica se o ID já existe na árvore
    while(consulta(arvore->raiz, paciente.id) != NULL) {
        paciente.id = clock() % UINT_MAX;
    }

    // Adiciona o registro ao arquivo de dados
    size_t offset = fappend(&paciente, sizeof(Paciente), obter_arquivo_dados(PACIENTE));
    insere(arvore, paciente.id, offset);
    salvar_arvore( obter_arquivo_indices(PACIENTE), arvore);

    return paciente.id;
}