#ifndef B_TREE_DISC_H
#define B_TREE_DISC_H
#include "b_tree.h"
#include <stdbool.h>

// Funções para salvar e carregar a árvore B em disco
bool salvar_arvore(FILE *arquivo, bTree *arvore);
size_t salvar_nodo(FILE *arq, nodoBTree *n_, int ordem);
bTree *carregar_arvore(FILE *arquivo);
nodoBTree *carregar_nodo(FILE *arquivo, size_t offset, nodoBTree *pai, int ordem);
bTree *__copia_descritor(bTree *a);
nodoBTree *__copia_nodo(nodoBTree *n_, int ordem);

#endif