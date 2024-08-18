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

    // Escreve a estrutura do arquivo invertido no arquivo
    salvarArquivoInvertido(ai);

    // Retorna o ponteiro para a estrutura criada
    return ai;
}

/**
 * Salva a estrutura do arquivo invertido no arquivo de postings.
 *
 * @param arquivo Ponteiro para o ArquivoInvertido que será salvo.
 *
 */
bool salvarArquivoInvertido(ArquivoInvertido *arquivo) {
    // Verifica se o arquivo é nulo
    if (arquivo == NULL) {
        print(LOG_ERROR, "Arquivo invertido nulo");
        return false;
    }

    // Volta para o início do arquivo de postings
    fseek(arquivo->arquivo_posting, 0, SEEK_SET);

    // Escreve a estrutura do arquivo invertido no arquivo
    fwrite(arquivo, sizeof(ArquivoInvertido), 1, arquivo->arquivo_posting);

    // Escreve os índices no arquivo
    fwrite(arquivo->indices, sizeof(IndiceInvertido), arquivo->__num_indices_max,
           arquivo->arquivo_posting);

    return true;
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
    if (i > f)
        return NULL;

    // Calcula o índice do meio
    int meio = (i + f) / 2;

    // Realiza a busca binária
if (a->indices[meio].chave < c)
    return __consultaArquivoInvertido(a, c, meio + 1, f); 

if (a->indices[meio].chave > c)
    return __consultaArquivoInvertido(a, c, i, meio - 1);

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
    if (arquivo == NULL || offset < 0) {
        print(LOG_ERROR, "Arquivo inválido ou offset negativo");
        return NULL;
    }

    // Aloca memória para o posting
    Posting *p = calloc(1, sizeof(Posting));
    if (p == NULL) {
        print(LOG_ERROR, "Erro ao alocar memória para Posting");
        return NULL;
    }

    // Vai para o offset do posting no arquivo
    if (fseek(arquivo->arquivo_posting, offset, SEEK_SET) != 0) {
        print(LOG_ERROR, "Erro ao mover o ponteiro do arquivo para o offset %ld", offset);
        free(p);
        return NULL;
    }

    // Lê o posting
    if (fread(p, sizeof(Posting), 1, arquivo->arquivo_posting) != 1) {
        print(LOG_ERROR, "Erro ao ler o Posting");
        free(p);
        return NULL;
    }

    // Aloca memória para os registros
    p->registros = calloc(arquivo->__tamanho_posting, sizeof(keytype));
    if (p->registros == NULL) {
        print(LOG_ERROR, "Erro ao alocar memória para os registros");
        free(p);
        return NULL;
    }

    // Lê os registros
    if (fread(p->registros, sizeof(keytype), arquivo->__tamanho_posting, arquivo->arquivo_posting) != arquivo->__tamanho_posting) {
        print(LOG_ERROR, "Erro ao ler os registros");
        free(p->registros);
        free(p);
        return NULL;
    }

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
    if (p == NULL) {
        print(LOG_ERROR, "Erro ao carregar o primeiro posting");
        return NULL;
    }

    // Carrega o próximo posting, se existir
    Posting *atual = p;
    while (atual->prox != NULL) {
        atual->prox = carregarPosting_Offset(arquivo, (long)atual->prox);
        if (atual->prox == NULL) {
            print(LOG_ERROR, "Erro ao carregar o próximo posting");
            liberaPosting(p);
            return NULL;
        }
        atual = atual->prox;
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

        indice = inserirIndice(arquivo, chave, adicionarPosting(arquivo));
        if (indice == NULL) {
            print(LOG_ERROR, "Erro ao inserir novo índice");
            return false;
        }

        salvarArquivoInvertido(arquivo);
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
                p->registros[i] = registro;
                inserido = true;

                // Salva o posting no arquivo
                if (!salvarPosting(arquivo, p, offset)) {
                    print(LOG_ERROR, "Erro ao salvar posting");
                    return false;
                }

                return true;
            }
        }

        // Se existir um próximo posting, carrega o próximo
        if (p->prox != NULL) {
            offset = (long)carregarPosting_Offset(arquivo, (long)p->prox)->prox;
        } else {

            // Se não existir um próximo posting, cria um novo
            long offset_novo = adicionarPosting(arquivo);
            if (offset_novo == -1) {
                print(LOG_ERROR, "Erro ao adicionar novo posting");
                return false;
            }
            Posting *p_novo = carregarPosting_Offset(arquivo, offset);
            p_novo->registros[0] = registro;
            salvarPosting(arquivo, p_novo, offset_novo);

            // Sobrescreve o próximo offset do posting atual no arquivo
            fseek(arquivo->arquivo_posting, offset, SEEK_SET); // Vai para o posting atual
            fwrite(&offset_novo, sizeof(Posting *), 1,
                   arquivo->arquivo_posting); // Sobrescreve o offset
        }
    }
    return true;
}

/**
 * Insere um índice no arquivo invertido.
 *
 * @param arquivo Ponteiro para o ArquivoInvertido onde o índice será inserido.
 * @param chave Chave associada ao índice.
 * @param posting Offset do arquivo onde o posting está armazenado.
 *
 * @return Ponteiro para o índice inserido, ou NULL em caso de erro.
 *
 */
IndiceInvertido *inserirIndice(ArquivoInvertido *arquivo, int chave, long posting) {
    // Verifica se o arquivo é nulo
    if (arquivo == NULL || chave < 0 || posting < 0) {
        print(LOG_ERROR, "Parâmetros inválidos");
        return NULL;
    }

    // Verifica se o número máximo de índices foi atingido
    if (arquivo->num_indices >= arquivo->__num_indices_max) {
        print(LOG_ERROR, "Número máximo de índices atingido");
        return NULL;
    }

    // Insere fazendo inserção direta (ordenado)
    int i = arquivo->num_indices;
    while (i > 0 && arquivo->indices[i - 1].chave > chave) {
        arquivo->indices[i] = arquivo->indices[i - 1];
        i--;
    }

    // Insere o novo índice
    IndiceInvertido *indice = &arquivo->indices[i];
    indice->chave = chave;
    indice->posting = posting;

    // Incrementa o número de índices
    arquivo->num_indices++;

    return indice;
}

/**
 * Adiciona um posting ao arquivo
 *
 * @param arquivo Ponteiro para o ArquivoInvertido onde o posting será adicionado.
 *
 * @return Offset do posting no arquivo, ou -1 em caso de erro.
 */
long adicionarPosting(ArquivoInvertido *arquivo) {
    if (arquivo == NULL)
        return -1;

    fseek(arquivo->arquivo_posting, 0, SEEK_END);
    long offset = ftell(arquivo->arquivo_posting);
    Posting p = {0};
    if (fwrite(&p, sizeof(Posting), 1, arquivo->arquivo_posting) != 1) {
        return -1;
    }
    
    // Aloca memória para os registros
    keytype *registros = calloc(arquivo->__tamanho_posting, sizeof(keytype));
    if (registros == NULL) {
        print(LOG_ERROR, "Erro ao alocar memória para os registros");
        return -1;
    }

    // Escreve os registros no arquivo
    if (fwrite(registros, sizeof(keytype), arquivo->__tamanho_posting, arquivo->arquivo_posting) !=
        arquivo->__tamanho_posting) {
        free(registros);
        return -1;
    }

    free(registros);

    return offset;
}

/**
 * Salva um posting no arquivo
 *
 * @param arquivo Ponteiro para o ArquivoInvertido onde o posting será salvo.
 * @param posting Ponteiro para o Posting que será salvo.
 * @param offset Offset do posting no arquivo.
 *
 * @return true se a operação for bem-sucedida, ou false em caso de erro.
 */

bool salvarPosting(ArquivoInvertido *arquivo, Posting *posting, long offset) {
    if (posting == NULL)
        return false;

    fseek(arquivo->arquivo_posting, offset, SEEK_SET);

    // Salva o posting
    if (fwrite(posting, sizeof(Posting), 1, arquivo->arquivo_posting) != 1) {
        return false;
    }

    // Salva os registros
    if (fwrite(posting->registros, sizeof(keytype), arquivo->__tamanho_posting,
               arquivo->arquivo_posting) != arquivo->__tamanho_posting) {
        return false;
    }

    return true;
}

/**
 * Carregar Arquivo Invertido do disco
 *
 * @param nome_arquivo Nome do arquivo invertido
 *
 * @return Ponteiro para o arquivo invertido carregado, ou NULL em caso de erro.
 *
 */
ArquivoInvertido *carregarArquivoInvertido(char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (arquivo == NULL) {
        print(LOG_ERROR, "Erro ao abrir o arquivo invertido");
        return NULL;
    }

    ArquivoInvertido *ai = calloc(sizeof(ArquivoInvertido), 1);
    if (ai == NULL) {
        print(LOG_ERROR, "Erro ao alocar memória para o arquivo invertido");
        fclose(arquivo);
        return NULL;
    }

    // Lê o arquivo invertido
    fread(ai, sizeof(ArquivoInvertido), 1, arquivo);

    // Lê os índices
    ai->indices = calloc(sizeof(IndiceInvertido), ai->__num_indices_max);
    if (ai->indices == NULL) {
        print(LOG_ERROR, "Erro ao alocar memória para os índices");
        fclose(arquivo);
        free(ai);
        return NULL;
    }

    fread(ai->indices, sizeof(IndiceInvertido), ai->__num_indices_max, arquivo);

    ai->arquivo_posting = arquivo;

    return ai;
}

/**
 * Fecha o arquivo invertido
 *
 * @param arquivo Ponteiro para o arquivo invertido que será fechado.
 *
 * @return true se a operação for bem-sucedida, ou false em caso de erro.
 */
bool fecharArquivoInvertido(ArquivoInvertido *arquivo) {
    if (arquivo == NULL)
        return true;

    fclose(arquivo->arquivo_posting);
    liberarArquivoInvertido(arquivo);
    return true;
}