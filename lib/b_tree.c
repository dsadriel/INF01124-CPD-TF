#include <b_tree.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


 /**
 * Cria um novo nó da B-tree com base na ordem fornecida e o pai indicado.
 *
 * @param ordem A ordem da B-tree, que determina o número MÍNIMO DE CHAVES.
 * @param pai O ponteiro para o nodo pai do novo nodo. Pode ser NULL se o nó for a raiz.
 * @return Um ponteiro para o novo nodo criado.
 */

nodoBTree* criaNodo(int ordem, nodoBTree* pai){
    nodoBTree* nodo = (nodoBTree*)calloc(1, sizeof(nodoBTree));
    if (nodo == NULL) {
        printf("Falha ao alocar memória para o nodo!\n");
        exit(-1);
    }
    nodo->index = 0;
    nodo->pai = pai;
    nodo->num = 0;
    nodo->flagfolha = true;

    nodo->keys = (keytype*)calloc((ordem * 2 + 1), sizeof(keytype));
    if (nodo->keys == NULL) {
        printf("Falha ao alocar memória!\n");
        exit(-1);
    }

    nodo->filhos = (nodoBTree**)calloc((ordem * 2 + 2), sizeof(nodoBTree*));
    if (nodo->filhos == NULL) {
        printf("Falha ao alocar memória!\n");
        exit(-1);
    }

    for (int i = 0; i < ordem *2 + 2 ; i++){
        nodo->filhos[i] = NULL;
    }
    return nodo;
}

 /**
 * Cria uma nova B-tree com base na ordem fornecida e inicializa a raiz.
 *
 * @param ordem A ordem da B-tree, que determina o número máximo de chaves e filhos.
 * @return Um ponteiro para a nova árvore B-tree criada.
 */

bTree* criaArv(int ordem){

    bTree* arv = (bTree*)calloc(1, sizeof(bTree));
    if (arv == NULL) {
        printf("Falha ao alocar memória!\n");
        exit(-1);
    }

    arv->raiz = criaNodo(ordem,NULL);
    arv->ordem = ordem;

    return arv;
}

/**
 * Divide um nodo da B-tree que está cheio e promove a chave do meio para o pai.
 *
 * @param arv A B-tree na qual o nodo está localizado.
 * @param esquerdo O nodo a ser dividido. Este nodo deve estar cheio.
 */

void splitNodo(bTree* arv, nodoBTree* esquerdo){
    
    int ordem = arv->ordem;
    int index = esquerdo->index;
    // se entrou aqui, é por que tem 1 elemento a mais do que pode no nodo
    // nodo deve ser dividido em dois e key do meio promovida

    //  nodo sem pai: cria nodo pai (leva ao fim da recursao)
    if (esquerdo->pai == NULL){
        nodoBTree* novoPai = criaNodo(ordem, NULL);
        novoPai->flagfolha = false;
        novoPai->filhos[0] = esquerdo;
        arv->raiz = novoPai;
        esquerdo->pai = novoPai;
    }

    //  cria nodo direito
    nodoBTree* direito= criaNodo(ordem,esquerdo->pai);
    direito->flagfolha = esquerdo->flagfolha;
    direito->num = ordem;
    
    //  nodo direito recebe t maiores valores do esquerdo
	for(int j = 0; j < ordem; j++){
        direito->keys[j] = esquerdo->keys[j+ordem+1];
	}
    if(!(direito->flagfolha)){
		for(int j = 0; j < ordem + 1; j++){
			direito->filhos[j] = esquerdo->filhos[j+ordem+1];
		}
	}    
    
    //  nodo esquerdo fica com os t menores valores
    esquerdo->num = ordem;

    // seleciona chave do meio
    keytype c = esquerdo->keys[ordem];

    // ajeita filhos do pai
	for(int j = esquerdo->pai->num; j >= index + 1; j--){
        esquerdo->pai->filhos[j]->index++;
		esquerdo->pai->filhos[j+1] = esquerdo->pai->filhos[j];
	}
    
	// liga-se o novo filho ao pai
	esquerdo->pai->filhos[index + 1] = direito;
    direito->index = index+1;

    //  nodo pai recebe o meio 
	for(int j = esquerdo->pai->num - 1; j >= index; j--){
		esquerdo->pai->keys[j + 1] = esquerdo->pai->keys[j];
	}

    esquerdo->pai->keys[index] = c;
    esquerdo->pai->num++;

    //se pai ficar cheio
    if (esquerdo->pai->num > 2*ordem){
        splitNodo(arv, esquerdo->pai);
    }

}

/**
 * Insere uma chave na B-tree recursivamente, começando a partir do nodo fornecido.
 * Se o nodo estiver cheio após a inserção, o nodo é dividido.
 *
 * @param arv A B-tree onde a chave deve ser inserida.
 * @param nodo O nodo onde a chave deve ser inserida ou onde a inserção deve continuar.
 * @param c A chave a ser inserida, representada por um tipo `keytype`.
 */

void insereAux(bTree* arv, nodoBTree* nodo, keytype c){
    int key = c.key;
    int i = nodo->num-1;
    int ordem = arv->ordem;

    // parada da recursividade: se for folha
    if (nodo->flagfolha){
        while(i >= 0 && nodo->keys[i].key > key){
            nodo->keys[i+1] = nodo->keys[i];
            i--;
        }
        nodo->keys[i+1] = c;
        nodo->num++;

        //se ficar cheio
        if (nodo->num > 2*ordem){
            splitNodo(arv, nodo);
        }
    }
    else{
        // busca nos filhos
        while(i >= 0 && nodo->keys[i].key > key){
            i--;
        }
        insereAux(arv,nodo->filhos[i+1], c);
    }

}

/**
 * Insere uma nova chave na B-tree. Se a raiz estiver vazia, a chave é inserida diretamente na raiz.
 * Caso contrário, a inserção é realizada usando a função `insereAux`.
 *
 * @param arv A B-tree onde a chave deve ser inserida.
 * @param key O valor da chave a ser inserida.
 * @param offset Um valor associado à chave, representado por um tipo `int`.
 */

void insere(bTree* arv, int key, int offset){
    keytype c;
    c.key = key;
    c.offset = offset;

    // se a raiz for vazia
    if (arv->raiz->num == 0){
        // insere na raiz
        arv->raiz->keys[0] = c;
        arv->raiz->num++;
    }
    else{
        insereAux(arv, arv->raiz, c);
    }


}

/**
 * Libera a memória de um nodo da B-tree.
 *
 * @param nodo O nodo a ser liberado.
 */

void liberaNodo(nodoBTree* nodo) {
    if (nodo != NULL) {
        // libera os filhos recursivamente, se não for folha
        if (!nodo->flagfolha) {
            for (int i = 0; i <= nodo->num; i++) {
                liberaNodo(nodo->filhos[i]);
            }
        }

        // libera memória para as chaves e filhos
        free(nodo->keys);
        free(nodo->filhos);
        free(nodo);
    }
}

/**
 * Libera a memória da B-tree.
 *
 * @param arv A árvore a ser liberada.
 */

void liberaArv(bTree* arv) {
    if (arv != NULL) {
        // libera a memória da raiz
        liberaNodo(arv->raiz);
        // libera a memória da estrutura da árvore
        free(arv);
    }
}

/**
 * Consulta uma chave na B-tree.
 *
 * @param raiz O nodo raiz da árvore.
 * @param key A chave a ser consultada.
 * @return O ponteiro `keytype` onde a chave foi encontrada ou NULL se a chave não estiver na árvore.
 */
keytype* consulta(nodoBTree* raiz, int key) {
    int i = 0;

    // encontra a posição da chave no nodo atual
    while (i < raiz->num && key > raiz->keys[i].key) {
        i++;
    }

    //  se a chave foi encontrada 
    if (i < raiz->num && key == raiz->keys[i].key) {
        return &raiz->keys[i];  // retorna 
    }

    // se é folha e a chave não foi encontrada, retorna NULL
    if (raiz->flagfolha) {
        return NULL;
    }

    // busca no filho
    return consulta(raiz->filhos[i], key);
}
