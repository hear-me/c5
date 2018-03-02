#ifndef SCRIPT_COMMON_H
#define SCRIPT_COMMON_H

#include "script.h"

struct host {
    const char * name;
    void (*importable_register)(
            const char *,
            const struct script * (*)()
        );
};

__attribute__((weak)) const char * HOSTNAME = "default";

#endif /* SCRIPT_COMMON_H */
