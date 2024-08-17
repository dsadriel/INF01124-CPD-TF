#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdarg.h>

// Define as cores para uso no console
#define RESET   "\033[0m"
#define RED     "\033[31m"      // Error
#define YELLOW  "\033[33m"      // Warning
#define GREEN   "\033[32m"      // Info

typedef enum {
    LOG_NONE,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} log_level;


// Funções de utilidade
void print(log_level level, const char *msg, ...);

#endif