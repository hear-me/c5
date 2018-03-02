#ifndef SCRIPT_IMPORTABLE_H
#define SCRIPT_IMPORTABLE_H

#include "common.h"

#define NULL_EXTENSION { .id = 0, .name = NULL, .data = NULL }

extern const struct host * host;

#define IMPORTABLE(name) \
    __attribute__((constructor(1001))) static void init() { \
        host->importable_register(name, &import); \
    }

#endif /* SCRIPT_IMPORTABLE_H */
