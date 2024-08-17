#include "b_tree_disc.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

/**
 * Salva uma árvore B em um arquivo do disco.
 *
 * @param arquivo ponteiro para o arquivo destino.
 * @param arvore__ ponteiro para uma bTree
 *
 * @returns boolean sobre o sucesso do arquivo.
 * É necessário implementar o tratamento em caso de erro
 * pois o arquivo não é limpo nestes casos.
 */
bool salvar_arvore(FILE *arquivo, bTree *arvore__) {
    if (arquivo == NULL || arvore__ == NULL)
        return false;

    // Cria uma cópia da árvore para manipulação
    bTree *arvore = __copia_descritor(arvore__);
    if (arvore == NULL) {
        print(LOG_ERROR, "Erro ao copiar a árvore\n");
        return false;
    }

    if (arvore->raiz != NULL) {
        // Pula o espaço dedicado para o descritor da árvore
        fseek(arquivo, sizeof(bTree), SEEK_SET);

        // Salva o nodo raiz
        size_t offset_raiz = salvar_nodo(arquivo, arvore->raiz, arvore->ordem);
        if (offset_raiz == 0) {
            print(LOG_ERROR, "Erro ao salvar a raiz\n");
            return false;
        }

        // Atualiza o ponteiro com o offset no arquivo
        arvore->raiz = (void *)offset_raiz;
    } else {
        arvore->raiz = 0;
    }

    // Volta para o início do arquivo
    fseek(arquivo, 0, SEEK_SET);

    // Salva o descritor no arquivo
    if (fwrite(arvore, sizeof(bTree), 1, arquivo) != 1) {
        print(LOG_ERROR, "Erro ao salvar o descritor da árvore\n");
        return false;
    }

    // Libera a memória alocada
    free(arvore);


    return true; // Operação realizada com sucesso!
}

/**
 * Salva um nodo e seus filhos em um arquivo do disco.
 *
 * @param arq ponteiro para o  arquivo onde os nodos serão salvos
 * @param n_ ponteiro para o nodo que deve ser salvo
 * @param ordem ordem da árvore B
 *
 * @return offset do nodo salvo na memória
 */
size_t salvar_nodo(FILE *arq, nodoBTree *n_, int ordem) {
    if (n_ == NULL)
        return 0; // Não existe nodo a ser salvo

    int max_chaves = ordem * 2 + 1;
    int max_filhos = max_chaves + 1;

    // Cria uma cópia do nodo para manipulação
    nodoBTree *n = __copia_nodo(n_, ordem);

    // Salva os nodos filhos e atualiza os ponteiros do nodo com o offset de cada um
    for (int i = 0; i < max_filhos; i++) {
        n->filhos[i] = (void *)salvar_nodo(arq, n->filhos[i], ordem);
    }

    // Salva o nodo
    size_t offset_nodo = ftell(arq);
    if (fwrite(n, sizeof(nodoBTree), 1, arq) != 1) {
        print(LOG_ERROR, "Erro ao salvar o nodo\n");
        return 0;
    }

    // Salva as chaves
    if (fwrite(n->keys, sizeof(keytype), max_chaves, arq) != max_chaves) {
        print(LOG_ERROR, "Erro ao salvar as chaves\n");
        return 0; // Erro ao salvar as chaves
    }

    // Salva os filhos
    if (fwrite(n->filhos, sizeof(nodoBTree *), max_filhos, arq) != max_filhos) {
        print(LOG_ERROR, "Erro ao salvar os filhos\n");
        return 0; // Erro ao salvar os filhos
    }

    // Libera a memória alocada
    free(n->keys);
    free(n->filhos);
    free(n);

    return offset_nodo;
}

/**
 * Carrega uma árvore B de um arquivo do disco.
 *
 * @param arquivo ponteiro para o arquivo de origem.
 *
 * @returns ponteiro para a árvore carregada, ou NULL em caso de erro.
 */
bTree *carregar_arvore(FILE *arquivo) {
    if (arquivo == NULL) {
        print(LOG_ERROR, "Arquivo inválido\n");
        return NULL;
    }

    bTree *a = malloc(sizeof(bTree));
    if (a == NULL) {
        print(LOG_ERROR, "Erro ao alocar memória para a árvore\n");
        return NULL;
    }

    fseek(arquivo, 0, SEEK_SET);
    if (fread(a, sizeof(bTree), 1, arquivo) != 1) {
        print(LOG_ERROR, "Erro ao ler o descritor da árvore\n");
        free(a);
        return NULL; // Não foi possível carregar a árvore;
    }

    a->raiz = carregar_nodo(arquivo, (size_t)a->raiz, NULL, a->ordem);
    if (a->raiz == NULL && a->ordem > 0) {
        print(LOG_ERROR, "Erro ao carregar a raiz da árvore\n");
        free(a);
        return NULL;
    }

    return a;
}

/**
 * Carrega um nodo de uma árvore B a partir de um arquivo.
 *
 * @param arquivo ponteiro para o arquivo de origem.
 * @param offset posição do nodo no arquivo.
 * @param pai ponteiro para o nodo pai (se houver).
 * @param ordem ordem da árvore B.
 *
 * @returns ponteiro para o nodo carregado, ou NULL em caso de erro.
 */
nodoBTree *carregar_nodo(FILE *arquivo, size_t offset, nodoBTree *pai, int ordem) {
    if (offset == 0)
        return NULL;

    int max_chaves = ordem * 2 + 1;
    int max_filhos = max_chaves + 1;

    // Move o cursor do arquivo para o offset correto
    fseek(arquivo, offset, SEEK_SET);

    // Aloca a memória
    nodoBTree *nodo = calloc(1, sizeof(nodoBTree));
    if (nodo == NULL) {
        printf("Erro ao alocar memória para o nodo\n");
        return NULL;
    }

    keytype *chaves = calloc(max_chaves, sizeof(keytype));
    if (chaves == NULL) {
        printf("Erro ao alocar memória para as chaves\n");
        free(nodo);
        return NULL;
    }

    // Lê os filhos
    nodoBTree **filhos = calloc(max_filhos, sizeof(nodoBTree *));
    if (filhos == NULL) {
        printf("Erro ao alocar memória para os filhos\n");
        free(nodo->keys);
        free(nodo);
        return NULL;
    }

    // Lê o nodo
    if (fread(nodo, sizeof(nodoBTree), 1, arquivo) != 1) {
        printf("Erro ao ler o nodo\n");
        free(nodo);
        free(chaves);
        return NULL;
    }

    // Lê as chaves
    if (fread(chaves, sizeof(keytype), max_chaves, arquivo) != max_chaves) {
        printf("Erro ao ler as chaves\n");
        free(nodo);
        free(chaves);
        return NULL;
    }
    nodo->keys = chaves;

    // Lê os filhos
    if (fread(filhos, sizeof(size_t), max_filhos, arquivo) != max_filhos) {
        printf("Erro ao ler os filhos\n");
        free(nodo->keys);
        free(nodo);
        free(filhos);
        return NULL;
    }
    nodo->filhos = filhos;

    // Lê os filhos
    for (int i = 0; i < max_filhos; i++) {
        size_t offset_nodo = (size_t)nodo->filhos[i];
        nodo->filhos[i] = carregar_nodo(arquivo, offset_nodo, nodo, ordem);
        if (nodo->filhos[i] != NULL) {
            nodo->filhos[i]->pai = nodo;
        }
    }

    return nodo;
}


/**
 * Cria uma cópia de um nodo da árvore B para manipulação.
 * 
 * @param n_ ponteiro para o nodo a ser copiado.
 * @param ordem ordem da árvore B.
 * 
 * @returns ponteiro para o novo nodo criado.
 * 
 */
nodoBTree *__copia_nodo(nodoBTree *n_, int ordem) {
    if (n_ == NULL)
        return NULL;

    int max_chaves = ordem * 2 + 1;
    int max_filhos = max_chaves + 1;

    nodoBTree *n = calloc(1, sizeof(nodoBTree));
    if (n == NULL) {
        printf("Erro ao alocar memória para o nodo\n");
        return NULL;
    }

    keytype *chaves = calloc(max_chaves, sizeof(keytype));
    if (chaves == NULL) {
        printf("Erro ao alocar memória para as chaves\n");
        free(n);
        return NULL;
    }

    nodoBTree **filhos = calloc(max_filhos, sizeof(nodoBTree *));
    if (filhos == NULL) {
        printf("Erro ao alocar memória para os filhos\n");
        free(chaves);
        free(n);
        return NULL;
    }

    memcpy(n, n_, sizeof(nodoBTree));
    memcpy(chaves, n_->keys, max_chaves * sizeof(keytype));
    memcpy(filhos, n_->filhos, max_filhos * sizeof(nodoBTree *));

    n->keys = chaves;
    n->filhos = filhos;
    
    return n;

}


/**
 * Cria uma cópia de um descritor de árvore B para manipulação.
 * 
 * @param a ponteiro para o descritor a ser copiado.
 * 
 * @returns ponteiro para o novo descritor criado.
 */

bTree *__copia_descritor(bTree *a) {
    if (a == NULL)
        return NULL;

    bTree *arvore = calloc(1, sizeof(bTree));
    if (arvore == NULL) {
        printf("Erro ao alocar memória para a árvore\n");
        return NULL;
    }

    memcpy(arvore, a, sizeof(bTree));
    arvore->raiz = __copia_nodo(a->raiz, a->ordem);

    return arvore;
}