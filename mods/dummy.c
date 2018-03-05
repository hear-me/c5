#include "importable.h"
//#include "mod/dummy.h"

#define MODNAME "dummy"
#include "error.h"

static const struct script * _import() {

    const static struct script script = {
        .unload = NULL,
        .create = NULL,
        .destroy = NULL,
        .static_event = NULL,
        .event = NULL,
        .extensions = {
            NULL_EXTENSION
        }
    };

    return &script;
}

IMPORTABLE(MODNAME)
