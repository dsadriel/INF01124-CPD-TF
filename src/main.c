#include <file_manager.h>
#include <stdio.h>

int main() {
    bool result = iniciar_file_manager();

    if (result) {
        printf("File manager iniciado com sucesso!\n");
    } else {
        printf("Falha ao iniciar file manager!\n");
    }
    return 0;
}