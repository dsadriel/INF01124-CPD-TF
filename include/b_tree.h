#ifndef B_TREE_H
#define B_TREE_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// TODO: Implementar a árvore B com uso de disco 

/**
 * Estrutura de dados que representa um nodo da B_TREE
 */

typedef struct {
    int key;
    int offset;
} keytype;
typedef struct nodo {
    int num;                // Representa o número de chaves armazenadas no nodo
    bool flagfolha;         // Indicador de o nodo é folha ou não
    keytype* keys;          // Vetor de chaves
    struct nodo** filhos;   // Vetor de ponteiros para nodos filhos
    struct nodo* pai;       // Ponteiro para o pai
    int index;              // Indicador de qual o index do nodo em relação ao pai
} nodoBTree;

/**
 * Estrutura de dados que representa uma B_TREE
 */
typedef struct {
    nodoBTree* raiz;        // Ponteiro para o nodo raiz da árvore
    int ordem;              // Representa a ordem da árvore (quantidade mínima de chaves em um nodo)
} bTree;

/**
 * Operações definidas para uma B_TREE
 */

// criar
bTree* criaArv (int ordem);
nodoBTree* criaNodo (int ordem, nodoBTree* pai);

// inserir
void insere(bTree* arv, int key, int offset);
void insereAux(bTree* arv, nodoBTree* nodo, keytype c);
void splitNodo(bTree* arv, nodoBTree* esquerdo);

// consultar
keytype* consulta(nodoBTree* raiz, int key);

// free
void liberaNodo(nodoBTree* nodo);
void liberaArv(bTree* arv);




#endif