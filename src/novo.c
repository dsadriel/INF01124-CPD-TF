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

int main(int argc, char *argv[]) {

    if (argc < 2 || tem_argumento(argc, argv, "-h") || tem_argumento(argc, argv, "--help")) {
        printf("Uso: %s <-a | -p>\n"
        "  -a: Cria um novo agendamento\n"
        "  -p: Cria um novo paciente\n", argv[0]);
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
    // Se a árvore não existir, cria uma nova
    if (arvore == NULL) {
        print(LOG_WARNING, "Árvore B de índices não encontrada. Criando nova árvore...\n");
        int ordem = tipo == AGENDAMENTO ? ORDEM_ARVORE_AGENDAMENTOS : ORDEM_ARVORE_PACIENTES;
        arvore = criaArv(ordem);
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
    size_t offset = fappend(&ag, sizeof(Agendamento), obter_arquivo_dados(AGENDAMENTO, true));
    insere(arvore, ag.id, offset);
    salvar_arvore( obter_arquivo_indices(AGENDAMENTO, true), arvore);

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
    size_t offset = fappend(&paciente, sizeof(Paciente), obter_arquivo_dados(PACIENTE, true));
    insere(arvore, paciente.id, offset);
    salvar_arvore( obter_arquivo_indices(PACIENTE, true), arvore);

    return paciente.id;
}