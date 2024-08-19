#include "b_tree_disc.h"
#include "entidades.h"
#include "entidades_utils.h"
#include "file_manager.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void atualizar_agendamento(int offset);
void atualizar_paciente(size_t offset);

int main(int argc, char const *argv[]) {

    if (argc < 3) {
        print(LOG_ERROR, "Uso: %s <-a | -p | -m | -r> <id>\n", argv[0]);
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

    // Busca o registro
    keytype *key = consulta(arvore->raiz, atoi(argv[2]));
    if (key == NULL) {
        print(LOG_ERROR, "Registro não encontrado.\n");
        return 1;
    }
    int offset = key->offset;

    // Solicita a atualização do registro
    switch (tipo) {
    case AGENDAMENTO:
        atualizar_agendamento(offset);
        break;
    case PACIENTE:
        atualizar_paciente(offset);
        break;
    case MEDICO:
        // atualizar_medico(offset);
        break;
    case RELATORIO:
        // atualizar_relatorio(offset);
        break;
    default:
        break;
    }

    return 0;
}

/**
 * Atualiza um registro de agendamento.
 *
 * @param offset Offset do registro no arquivo de dados
 */
void atualizar_agendamento(int offset) {
    Agendamento *agendamento = ler_agendamento_disco(offset);
    if (agendamento == NULL) {
        print(LOG_ERROR, "Erro ao ler o registro de agendamento\n");
        return;
    }

    // Solicita a atualização dos campos
    print(LOG_INFO, "Registro atual:\n");
    imprimir(AGENDAMENTO, agendamento);

    print(LOG_INFO, "Digite os novos dados do agendamento\n");

    // Recebe os novos dados do agendamento
    ler_entidade(AGENDAMENTO, agendamento);

    // Atualiza o registro
    // FIXME: Atualizar os demais indexadores
    FILE *arquivo = obter_arquivo_dados(AGENDAMENTO);
    fseek(arquivo, offset, SEEK_SET);
    fwrite(agendamento, sizeof(Agendamento), 1, arquivo);

    print(LOG_INFO, "Registro atualizado com sucesso\n");

    free(agendamento);
}


void atualizar_paciente(size_t offset){
    Paciente *paciente = ler_paciente_disco(offset);
    if (paciente == NULL) {
        print(LOG_ERROR, "Erro ao ler o registro de paciente\n");
        return;
    }

    // Solicita a atualização dos campos
    print(LOG_INFO, "Registro atual:\n");
    imprimir(PACIENTE, paciente);

    print(LOG_INFO, "Digite os novos dados do paciente\n");

    // Recebe os novos dados do paciente
    ler_entidade(PACIENTE, paciente);

    // Atualiza o registro
    // FIXME: Atualizar os demais indexadores
    FILE *arquivo = obter_arquivo_dados(PACIENTE);
    fseek(arquivo, offset, SEEK_SET);
    fwrite(paciente, sizeof(Paciente), 1, arquivo);

    print(LOG_INFO, "Registro atualizado com sucesso\n");
}