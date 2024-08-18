#include "arquivo_invertido.h"

// 

/**
 * Cria um arquivo invertido com um número especificado de índices.
 *
 * @param num_indices Número máximo de índices que o arquivo invertido poderá conter.
 * @param tamanho_posting Tamanho máximo de cada posting no arquivo.
 * @param nome_arquivo Nome do arquivo que será usado para armazenar os postings.
 *
 * @return Ponteiro para o ArquivoInvertido criado, ou NULL em caso de erro.
 */
ArquivoInvertido *criaArquivoInvertido(int num_indices, int tamanho_posting, char *nome_arquivo) {
    // Verifica se os valores de entrada são válidos
    if (num_indices < 1 || tamanho_posting < 1) {
        print(LOG_ERROR, "Valores de entrada inválidos. Número de Índices e Tamanho do Posting "
                         "devem ser maior iguais a 1");
        return NULL;
    }

    // Aloca memória para a estrutura do arquivo invertido
    ArquivoInvertido *ai = calloc(sizeof(ArquivoInvertido), 1);
    if (ai == NULL) {
        print(LOG_ERROR, "Falha ao alocar memória para criar o arquivo invertido");
        return NULL;
    }

    // Inicializa os campos da estrutura
    ai->__num_indices_max = num_indices; 
    ai->__tamanho_posting = tamanho_posting;

    // Aloca memória para os índices do arquivo invertido
    ai->indices = calloc(sizeof(IndiceInvertido), ai->__num_indices_max);
    if (ai->indices == NULL) {
        print(LOG_ERROR, "Falha ao alocar memória para os indices do arquivo invertido");
        free(ai);
        return NULL;
    }

    // Abre o arquivo para gravação e leitura
    ai->arquivo_posting = fopen(nome_arquivo, "wb+");
    if (ai->arquivo_posting == NULL) {
        print(LOG_ERROR, "Falha ao abrir o arquivo de posting");
        free(ai->indices);
        free(ai);
        return NULL;
    }

    // Retorna o ponteiro para a estrutura criada
    return ai;
}

/**
 * Libera a memória alocada para um arquivo invertido.
 *
 * @param arquivo Ponteiro para o ArquivoInvertido a ser liberado.
 *
 * @return true se a operação for bem-sucedida, ou false em caso de erro.
 */
bool liberarArquivoInvertido(ArquivoInvertido *arquivo) {
    // Verifica se o arquivo é nulo
    if (arquivo == NULL)
        return true;

    // Libera memória os índices do arquivo invertido
    free(arquivo->indices);
    // Libera a memória da estrutura principal
    free(arquivo);
    return true;
}

/**
 * Libera a memória alocada para um posting.
 *
 * @param posting Ponteiro para o Posting a ser liberado.
 *
 * @return true se a operação for bem-sucedida, ou false em caso de erro.
 */
bool liberaPosting(Posting *posting) {
    // Verifica se o posting é nulo
    if (posting == NULL)
        return true;

    // Libera o próximo posting, se existir
    if (posting->prox != NULL)
        liberaPosting(posting->prox);

    // Libera os registros e o próprio posting
    free(posting->registros);
    free(posting);
    return true;
}

/**
 * Consulta um registro no arquivo invertido com base em uma chave.
 *
 * @param arquivo Ponteiro para o ArquivoInvertido onde a consulta será realizada.
 * @param chave Chave que identifica o índice a ser consultado.
 *
 * @return Ponteiro para o Posting correspondente à chave, ou NULL se não encontrado.
 */
Posting *consultaArquivoInvertido(ArquivoInvertido *arquivo, int chave) {
    // Realiza a consulta binária para encontrar o índice
    IndiceInvertido *i = __consultaArquivoInvertido(arquivo, chave, 0, arquivo->num_indices);
    if (i == NULL)
        return NULL;

    // Carrega o posting associado ao índice encontrado
    Posting *p = carregarPosting(arquivo, i);

    return p;
}

/**
 * Função auxiliar para realizar a consulta binária em um arquivo invertido.
 *
 * @param a Ponteiro para o ArquivoInvertido onde a consulta será realizada.
 * @param c Chave que identifica o índice a ser consultado.
 * @param i Índice inicial do intervalo de busca.
 * @param f Índice final do intervalo de busca.
 *
 * @return Ponteiro para o IndiceInvertido correspondente à chave, ou NULL se não encontrado.
 */
IndiceInvertido *__consultaArquivoInvertido(ArquivoInvertido *a, int c, int i, int f) {
    // Verifica se o intervalo de busca é válido
    if (i < f)
        return NULL;

    // Calcula o índice do meio
    int meio = (i + f) / 2;

    // Realiza a busca binária
    if (a->indices[meio].chave < c)
        return __consultaArquivoInvertido(a, c, i, meio - 1);

    if (a->indices[meio].chave > c)
        return __consultaArquivoInvertido(a, c, meio + 1, f);

    if (a->indices[meio].chave == c)
        return &a->indices[meio];

    return NULL;
}

/**
 * Carrega um único posting do arquivo invertido.
 *
 * @param arquivo Ponteiro para o ArquivoInvertido que se refere ao posting.
 * @param offset O offset do posting no arquivo.
 *
 * @return Ponteiro para o Posting carregado, ou NULL em caso de erro.
 */
Posting *carregarPosting_Offset(ArquivoInvertido *arquivo, long offset) {
    // Verifica se os ponteiros de entrada são válidos
    if (arquivo == NULL || offset > 0)
        return NULL;

    // Aloca memória para o posting
    Posting *p = calloc(sizeof(Posting), 1);
    if (p == NULL) {
        print(LOG_ERROR, "Erro ao alocar memória");
        return NULL;
    }

    // Aloca memória para os registros
    p->registros = calloc(sizeof(keytype), arquivo->__tamanho_posting);
    if (p->registros == NULL) {
        print(LOG_ERROR, "Erro ao alocar memória para os registros");
        free(p);
        return NULL;
    }

    // Vai para o offset do posting no arquivo
    fseek(arquivo->arquivo_posting, offset, SEEK_SET);

    // Lê o posting e seus registros
    fread(p, sizeof(Posting), 1, arquivo->arquivo_posting);
    fread(p->registros, sizeof(keytype), arquivo->__tamanho_posting, arquivo->arquivo_posting);

    return p;
}

/**
 * Carrega um posting e seu próximo, se existir, a partir do arquivo invertido.
 *
 * @param arquivo Ponteiro para o ArquivoInvertido de onde os postings serão carregados.
 * @param indice Ponteiro para o IndiceInvertido associado ao primeiro posting.
 *
 * @return Ponteiro para o primeiro Posting carregado, ou NULL em caso de erro.
 */
Posting *carregarPosting(ArquivoInvertido *arquivo, IndiceInvertido *indice) {
    // Verifica se os ponteiros de entrada são válidos
    if (arquivo == NULL || indice == NULL)
        return NULL;

    // Carrega o primeiro posting
    Posting *p = carregarPosting_Offset(arquivo, indice->posting);

    // Carrega o próximo posting, se existir
    if (p->prox != NULL) {
        Posting *prox = carregarPosting(arquivo, p->prox);
        p->prox = prox;
    }

    // Retorna o primeiro posting
    return p;
}

/**
 * Insere um registro no arquivo invertido. Se a chave já existir, o registro é adicionado ao
 * posting existente. Se a chave não existir, um novo índice é criado e o registro é adicionado a um
 * novo posting.
 *
 * @param arquivo Ponteiro para o ArquivoInvertido onde o registro será inserido.
 * @param chave Chave associada ao registro.
 * @param registro Registro a ser inserido.
 *
 */
bool inserirRegistro(ArquivoInvertido *arquivo, int chave, keytype registro) {
    // Verifica se o arquivo é nulo
    if (arquivo == NULL || chave < 0) {
        print(LOG_ERROR, "Parâmetros inválidos");
        return false;
    }

    // Verifica se já existe um índice para a chave
    IndiceInvertido *indice = __consultaArquivoInvertido(arquivo, chave, 0, arquivo->num_indices);
    if (indice == NULL) {
        // Cria um novo índice
        if (arquivo->num_indices >= arquivo->__num_indices_max) {
            print(LOG_ERROR, "Número máximo de índices atingido");
            return false;
        }

        // TODO: Implementar a inserção de um novo índice
    }

    bool inserido = false;
    long offset = indice->posting;

    while (!inserido) {
        Posting *p = carregarPosting_Offset(arquivo, offset);
        if (p == NULL) {
            print(LOG_ERROR, "Erro ao carregar posting");
            return false;
        }
        
        // Busca um espaço vazio no posting
        for (int i = 0; i < arquivo->__tamanho_posting; i++) {
            if (p->registros[i].offset == 0) {
                p->registros[i] = registro; // Só foi na RAM
                inserido = true;
                

                // SALVAR NO DISCO
                break;
            }
        }



        // Se existir um próximo posting, carrega o próximo
        if (p->prox != NULL) {
            offset = carregarPosting_Offset(arquivo, p->prox)->prox;
            break;
        }

        // Se não existir um próximo posting, cria um novo
        long offset_novo = 1;//adicionarPosting(arquivo);
        if (offset_novo == -1) {
            print(LOG_ERROR, "Erro ao adicionar novo posting");
            return false;
        }

        // Sobrescreve o próximo offset do posting atual no arquivo
        fseek(arquivo->arquivo_posting, offset, SEEK_SET); // Vai para o posting atual
        fwrite(&offset_novo, sizeof(Posting*), 1, arquivo->arquivo_posting); // Sobrescreve o offset


    }
    
}
