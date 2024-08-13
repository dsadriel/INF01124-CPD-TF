//#include <file_manager.h>
#include <stdio.h>
#include <b_tree.h>

int main() {

    // // testes que eu fiz pra testar estrutura b_tree, caso queira verificar e fazer outros testes
    // bTree* uma_arv = criaArv(2);
    // insere(uma_arv,15,0);
    // insere(uma_arv,3,0);
    // insere(uma_arv,5,0);
    // insere(uma_arv,6,0);
    // insere(uma_arv,9,0);
    // insere(uma_arv,1,0);
    // insere(uma_arv,12,0);
    // insere(uma_arv,0,0);
    // insere(uma_arv,61,0);
    // insere(uma_arv,19,0);
    // insere(uma_arv,2,0);
    // insere(uma_arv,80,0);
    // insere(uma_arv,60,0);
    // insere(uma_arv,119,0);
    // insere(uma_arv,100,0);
    // insere(uma_arv,200,0);
    // insere(uma_arv,300,0);
    // printf("%p\n", consulta(uma_arv->raiz,20));
    // printf("%d\n", consulta(uma_arv->raiz,12)->key);
    // liberaArv(uma_arv);
    
    // // só mostro a primeira key do array para nao poluir a verificação
    // printf("%d\n",uma_arv->raiz->keys[0].key);
    // printf("%d\n",uma_arv->raiz->num);

    // printf("%d\n",uma_arv->raiz->filhos[0]->keys[0].key);
    // printf("%d\n",uma_arv->raiz->filhos[0]->num);

    // printf("%d\n",uma_arv->raiz->filhos[1]->keys[0].key);
    // printf("%d\n",uma_arv->raiz->filhos[1]->num);
   
    // printf("%d\n",uma_arv->raiz->filhos[0]->filhos[0]->keys[0].key);
    // printf("%d\n",uma_arv->raiz->filhos[0]->filhos[0]->num);

    // printf("%d\n",uma_arv->raiz->filhos[0]->filhos[1]->keys[0].key);
    // printf("%d\n",uma_arv->raiz->filhos[0]->filhos[1]->num);
   
    // printf("%d\n",uma_arv->raiz->filhos[0]->filhos[2]->keys[0].key);
    // printf("%d\n",uma_arv->raiz->filhos[0]->filhos[2]->num);
    
    // printf("%d\n",uma_arv->raiz->filhos[1]->filhos[0]->keys[0].key);
    // printf("%d\n",uma_arv->raiz->filhos[1]->filhos[0]->num);

    // printf("%d\n",uma_arv->raiz->filhos[1]->filhos[1]->keys[0].key);
    // printf("%d\n",uma_arv->raiz->filhos[1]->filhos[1]->num);
   
    // printf("%d\n",uma_arv->raiz->filhos[1]->filhos[2]->keys[0].key);
    // printf("%d\n",uma_arv->raiz->filhos[1]->filhos[2]->num);


    bool result = iniciar_file_manager();

    if (result) {
        printf("File manager iniciado com sucesso!\n");
    } else {
        printf("Falha ao iniciar file manager!\n");
    }
    return 0;

    
}