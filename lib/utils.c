#include "utils.h"


void print(log_level nivel, const char *__format, ...) {
    va_list args;
    va_start(args, __format);

    // Define o prefixo da mensagem de acordo com o nível
    switch (nivel) {
        case LOG_SUCCESS:
            printf(GREEN "[SUCESSO] " RESET);
            break;
        case LOG_WARNING:
            printf(YELLOW "[AVISO] " RESET);
            break;
        case LOG_ERROR:
            printf(RED "[ERRO] " RESET);
            break;
        case LOG_INFO:
        default:
            break;
    }

    // Imprime a mensagem formatada
    vprintf(__format, args);

    va_end(args);
}

/**
 * 
 */
bool tem_argumento(int argc, char **argv, char *arg) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], arg) == 0) {
            return true;
        }
    }
    return false;
}