#include "importable.h"

#include <stdio.h>

static void static_event(struct script * s, int ev, va_list args) {
    printf("mod_hello: static event '%d'\n", ev);
}

static void unload(struct script * script) {
    printf("Goodbye, %s from mod_hello!\n", host->name);
}

static const struct script * _import() {

    const static struct script script = {
        .unload = &unload,
        .create = NULL,
        .destroy = NULL,
        .static_event = &static_event,
        .event = NULL,
        .extensions = { NULL_EXTENSION }
    };

    printf("Hello, %s from mod_hello!\n", host->name);

    return &script;
}

IMPORTABLE("hello")
