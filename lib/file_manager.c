#include <file_manager.h>
#include <entidades_utils.h>

FILE *ARQUIVO_DADOS[NUM_ARQUIVOS] = {NULL};   // Arquivos de dados
FILE *ARQUIVO_INDICES[NUM_ARQUIVOS] = {NULL}; // Arquivos de índices
char *NOMES_ARQUIVOS[NUM_ARQUIVOS] = {        // Nomes dos arquivos de dados
    "data/pacientes.dat", "data/medicos.dat", "data/agendamentos.dat", "data/relatorios.dat"};
char *NOMES_INDICES[NUM_ARQUIVOS] = { // Nomes dos arquivos de índices
    "data/pacientes.idx", "data/medicos.idx", "data/agendamentos.idx", "data/relatorios.idx"};

/**
 * Retorna o ponteiro para o arquivo de dados de acordo com o tipo de entidade.
 *
 * @param tipo tipo da entidade
 *
 */
FILE *obter_arquivo_dados(TipoEntidade tipo) { return ARQUIVO_DADOS[tipo]; }

/**
 * Finaliza o file manager. Fecha os arquivos de pacientes, médicos, agendamentos e relatórios.
 *
 * @return void
 */
void finalizar_file_manager() {
    for (int i = 0; i < NUM_ARQUIVOS; i++) {
        if (ARQUIVO_DADOS[i] != NULL){}
            fclose(ARQUIVO_DADOS[i]);
        if (ARQUIVO_INDICES[i] != NULL)
            fclose(ARQUIVO_INDICES[i]);

        ARQUIVO_DADOS[i] = NULL;
        ARQUIVO_INDICES[i] = NULL;
    }
}

/**
 * Inicializa o file manager. Abre os arquivos de pacientes, médicos, agendamentos e relatórios.
 *
 * @return true se a inicialização foi bem sucedida, false caso contrário
 */
bool iniciar_file_manager() {
    for (size_t i = 0; i < NUM_ARQUIVOS; i++) {
        // Abre o arquivo de dados
        ARQUIVO_DADOS[i] = fopen(NOMES_ARQUIVOS[i], "r+b");
        if (ARQUIVO_DADOS[i] == NULL) {
            ARQUIVO_DADOS[i] = fopen(NOMES_ARQUIVOS[i], "w+b"); // Cria o arquivo se não existir
            if (ARQUIVO_DADOS[i] == NULL){
                finalizar_file_manager();
                return false;
            }
        }
        // Abre o arquivo de índices
        ARQUIVO_INDICES[i] = fopen(NOMES_INDICES[i], "r+b");
        if (ARQUIVO_INDICES[i] == NULL) {
            ARQUIVO_INDICES[i] = fopen(NOMES_INDICES[i], "w+b"); // Cria o arquivo se não existir
            if (ARQUIVO_INDICES[i] == NULL){
                finalizar_file_manager();
                return false;
            }
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
size_t obter_indice_arquivo(size_t id, TipoEntidade tipo) {
    FILE *arquivo = ARQUIVO_INDICES[tipo];
    if (arquivo == NULL)
        return -1;

    return -1; // TODO: Implementar a busca usando árvore B
}

/**
 * Lê um agendamento do arquivo de dados e retorna um ponteiro para a entidade.
 */
Agendamento *ler_agendamento(size_t id) {
    FILE *arquivo = ARQUIVO_DADOS[AGENDAMENTO];
    if (arquivo == NULL)
        return NULL;

    Agendamento *agendamento = malloc(sizeof(Agendamento));
    if (agendamento == NULL)
        return NULL;

    fseek(arquivo, id * sizeof(Agendamento), SEEK_SET);
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
Paciente *ler_paciente(size_t id) {
    FILE *arquivo = ARQUIVO_DADOS[PACIENTE];
    if (arquivo == NULL)
        return NULL;

    Paciente *paciente = malloc(sizeof(Paciente));
    if (paciente == NULL)
        return NULL;

    fseek(arquivo, id * sizeof(Paciente), SEEK_SET);
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
 * @return quantidade de elementos escritos
 */
size_t fappend(const void *content, size_t size, size_t amount, FILE *file) {
    size_t current = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t written = fwrite(content, size, amount, file);
    fseek(file, current, SEEK_SET);
    return written;
}
