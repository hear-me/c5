#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

__attribute__((unused)) static void error(const char * msg, ...) {
    va_list args;
    va_start(args, msg);
#ifdef MODNAME
    fprintf(stderr, "[ERROR] %s/%s: ", HOSTNAME, MODNAME);
#else /* !MODNAME */
#ifdef IS_IMPORTABLE
    fprintf(stderr, "[ERROR] %s/(importable): ", HOSTNAME);
#else
    fprintf(stderr, "[ERROR] %s: ", HOSTNAME);
#endif /* IS_IMPORTABLE */
#endif /* MODNAME */
    vfprintf(stderr, msg, args);
    va_end(args);

    exit(1);
}

__attribute__((unused)) static void warn(const char * msg, ...) {
    va_list args;
    va_start(args, msg);
#ifdef MODULE
    fprintf(stderr, "[WARN] %s/%s:", HOSTNAME, MODNAME);
#else /* !MODULE */
    fprintf(stderr, "[WARN] %s:", HOSTNAME);
#endif /* MODULE */
    vfprintf(stderr, msg, args);
    va_end(args);
}


#endif /* ERROR_H */
