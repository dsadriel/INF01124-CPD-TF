#include <file_manager.h>

FILE *ARQUIVO_DADOS[NUM_ARQUIVOS] = {NULL}; // Arquivos de dados
FILE *ARQUIVO_INDICES[NUM_ARQUIVOS] = {NULL}; // Arquivos de índices
char *NOMES_ARQUIVOS[NUM_ARQUIVOS] = { // Nomes dos arquivos de dados
    "data/pacientes.dat",
    "data/medicos.dat",
    "data/agendamentos.dat",
    "data/relatorios.dat"
};
char *NOMES_INDICES[NUM_ARQUIVOS] = { // Nomes dos arquivos de índices
    "data/pacientes.idx",
    "data/medicos.idx",
    "data/agendamentos.idx",
    "data/relatorios.idx"
};


/**
 * Finaliza o file manager. Fecha os arquivos de pacientes, médicos, agendamentos e relatórios.
 *
 * @return void
 */
void finalizar_file_manager() {
    for (int i = 0; i < 4; i++) {
        if (ARQUIVO_DADOS[i] != NULL) fclose(ARQUIVO_DADOS[i]);
        if (ARQUIVO_INDICES[i] != NULL) fclose(ARQUIVO_INDICES[i]);
    }
}

/**
 * Inicializa o file manager. Abre os arquivos de pacientes, médicos, agendamentos e relatórios.
 *
 * @return true se a inicialização foi bem sucedida, false caso contrário
 */
bool iniciar_file_manager() {
    for (size_t i = 0; i < 4; i++) {
        // Abre o arquivo de dados
        ARQUIVO_DADOS[i] = fopen(NOMES_ARQUIVOS[i], "r+b");
        if (ARQUIVO_DADOS[i] == NULL) {
            ARQUIVO_DADOS[i] = fopen(NOMES_ARQUIVOS[i], "w+b"); // Cria o arquivo se não existir
            if(ARQUIVO_DADOS[i] == NULL) return false;
        }
        // Abre o arquivo de índices
        ARQUIVO_INDICES[i] = fopen(NOMES_INDICES[i], "r+b");
        if (ARQUIVO_INDICES[i] == NULL) {
            ARQUIVO_INDICES[i] = fopen(NOMES_INDICES[i], "w+b"); // Cria o arquivo se não existir
            if(ARQUIVO_INDICES[i] == NULL) return false;
        }
    }

    return true;
}



/**
 * Obtém o índice de um arquivo de acordo com o id e o tipo de entidade fazendo uso do indexador.
 * 
 * @param id id da entidade
 * @param tipo tipo da entidade
 * 
 * @return índice da entidade no arquivo de dados
 */
size_t obter_indice_arquivo(size_t id, TipoEntidade tipo){
    FILE *arquivo = ARQUIVO_INDICES[tipo];
    if (arquivo == NULL) return -1;

    return -1; // TODO: Implementar a busca usando árvore B

}