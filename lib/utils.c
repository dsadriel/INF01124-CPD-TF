#include "utils.h"


void print(log_level level, const char *__format, ...) {
    va_list args;
    va_start(args, __format);

    // Select color and label based on log level
    switch (level) {
        case LOG_SUCCESS:
            printf(GREEN "[SUCCESS] " RESET);
            break;
        case LOG_WARNING:
            printf(YELLOW "[WARNING] " RESET);
            break;
        case LOG_ERROR:
            printf(RED "[ERROR] " RESET);
            break;
        case LOG_INFO:
        default:
            break;
    }

    // Print the formatted message
    vprintf(__format, args);

    va_end(args);
}