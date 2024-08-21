#include <entidades_utils.h>

#include <dirent.h>
#include <file_manager.h>
#include "utils.h"

FILE *ARQUIVO_DADOS[NUM_ARQUIVOS] = {NULL};   // Arquivos de dados
FILE *ARQUIVO_INDICES[NUM_ARQUIVOS] = {NULL}; // Arquivos de índices
char *NOMES_ARQUIVOS[NUM_ARQUIVOS] = {        // Nomes dos arquivos de dados
    "data/pacientes.dat", "data/medicos.dat", "data/agendamentos.dat", "data/relatorios.dat"};
char *NOMES_INDICES[NUM_ARQUIVOS] = { // Nomes dos arquivos de índices
    "data/pacientes.idx", "data/medicos.idx", "data/agendamentos.idx", "data/relatorios.idx"};

ArquivosAbertos *arquivos_abertos = NULL;

/**
 * Retorna o ponteiro para o arquivo de dados de acordo com o tipo de entidade.
 * Se o arquivo não estiver aberto, tenta abrir o arquivo. Se o arquivo não existir, cria um novo
 * arquivo.
 *
 * @param tipo tipo da entidade
 *
 */
FILE *obter_arquivo_dados(TipoEntidade tipo, bool criar) {
    if (ARQUIVO_DADOS[tipo] != NULL)
        return ARQUIVO_DADOS[tipo];

    // Abre o arquivo de dados, se existir
    ARQUIVO_DADOS[tipo] = fopen(NOMES_ARQUIVOS[tipo], "r+b");

    // Se o arquivo não existir, cria um novo arquivo
    if (ARQUIVO_DADOS[tipo] == NULL && criar) 
        ARQUIVO_DADOS[tipo] = fopen(NOMES_ARQUIVOS[tipo], "w+b"); // Cria o arquivo se não existir

    return ARQUIVO_DADOS[tipo];
}

/**
 * Retorna o ponteiro para o arquivo de índices de acordo com o tipo de entidade.
 *
 * @param tipo tipo da entidade
 *
 * @return ponteiro para o arquivo de índices
 */
FILE *obter_arquivo_indices(TipoEntidade tipo, bool criar) {
    if (ARQUIVO_INDICES[tipo] != NULL)
        return ARQUIVO_INDICES[tipo];

    // Abre o arquivo de índices, se existir
    ARQUIVO_INDICES[tipo] = fopen(NOMES_INDICES[tipo], "r+b");

    // Se o arquivo não existir, cria um novo arquivo
    if (ARQUIVO_INDICES[tipo] == NULL && criar)
        ARQUIVO_INDICES[tipo] = fopen(NOMES_INDICES[tipo], "w+b"); // Cria o arquivo se não existir

    return ARQUIVO_INDICES[tipo];
}


/**
 * Finaliza o file manager. Fecha todos os arquivos de dados e índices.
 *
 * @return void
 */
void finalizar_file_manager() {
    for (int i = 0; i < NUM_ARQUIVOS; i++) {
        if (ARQUIVO_DADOS[i] != NULL) {
        }
        fclose(ARQUIVO_DADOS[i]);
        if (ARQUIVO_INDICES[i] != NULL)
            fclose(ARQUIVO_INDICES[i]);

        ARQUIVO_DADOS[i] = NULL;
        ARQUIVO_INDICES[i] = NULL;
    }

    // Fecha todos os arquivos abertos (com sufixo)
    ArquivosAbertos *atual = arquivos_abertos;
    while (atual != NULL) {
        fclose(atual->arquivo);
        free(atual->nome);
        ArquivosAbertos *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}

/**
 * Lê um agendamento do arquivo de dados e retorna um ponteiro para a entidade.
 */
Agendamento *ler_agendamento_disco(size_t offset) {
    FILE *arquivo = obter_arquivo_dados(AGENDAMENTO, false);
    if (arquivo == NULL){
        print(LOG_ERROR, "Erro ao abrir o arquivo de agendamentos\n");
        return NULL;
    }

    Agendamento *agendamento = malloc(sizeof(Agendamento));
    if (agendamento == NULL)
        return NULL;

    fseek(arquivo, offset, SEEK_SET);
    fread(agendamento, sizeof(Agendamento), 1, arquivo);

    return agendamento;
}

/**
 * Lê um paciente do arquivo de dados e retorna um ponteiro para a entidade.
 *
 * @param id id do paciente
 *
 * @return ponteiro para o paciente
 * @return NULL se não foi possível ler o paciente
 */
Paciente *ler_paciente_disco(size_t offset) {
    FILE *arquivo = obter_arquivo_dados(PACIENTE, false);
    if (arquivo == NULL){
        print(LOG_ERROR, "Erro ao abrir o arquivo de pacientes\n");
        return NULL;
    }

    Paciente *paciente = malloc(sizeof(Paciente));
    if (paciente == NULL)
        return NULL;

    fseek(arquivo, offset, SEEK_SET);
    fread(paciente, sizeof(Paciente), 1, arquivo);

    return paciente;
}

/**
 * Insere um dado no final de um arquivo.
 *
 * @param content ponteiro para o conteúdo a ser inserido
 * @param size tamanho de cada elemento
 * @param amount quantidade de elementos
 * @param file ponteiro para o arquivo
 *
 * @return offset do arquivo após a inserção
 */
size_t fappend(const void *content, size_t size, FILE *file) {
    size_t current = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t offset = ftell(file);
    size_t written = fwrite(content, size, 1, file);
    fseek(file, current, SEEK_SET);
    return written == 1 ? offset : -1; // Retorna -1 se não foi possível escrever
}

/**
 * Apaga todos os dados dos arquivos de dados e índices.
 *
 * @return void
 */

void apagar_todos_os_dados() {

    // Abre o diretório
    DIR *pasta_data = opendir("data");

    if (pasta_data == NULL) {
        printf("Erro ao abrir o diretório data\n");
        return;
    }

    struct dirent *ent;
    while ((ent = readdir(pasta_data)) != NULL) {
        // Ignora os diretórios . e ..
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;

        // Monta o caminho do arquivo
        char *path = malloc(strlen("data/") + strlen(ent->d_name) + 1);
        strcpy(path, "data/");
        strcat(path, ent->d_name);

        // Apaga o arquivo
        remove(path);
        free(path);
    }

    // Fecha o diretório
    closedir(pasta_data);
}