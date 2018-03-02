#include "importable.h"
#include "mod/hello2.h"

#define MODNAME "mod_hello2"
#include "error.h"

#include <stdio.h>

static void unload(struct script * script) {
    printf("Goodbye, %s from mod_hello2!\n", host->name);
}

static void foo() {
    printf("mod_hello2: extension function!\n");
}

static void static_event(struct script * script, int ev, va_list args) {
    error("dummy error %d\n", ev);
}

static const struct script * import() {

    const static struct script script = {
        .unload = &unload,
        .create = NULL,
        .destroy = NULL,
        .static_event = &static_event,
        .event = NULL,
        .extensions = {
            { .id = MOD_HELLO2_EXT_FOO, .name = "foo", .data = (void*)&foo },
            NULL_EXTENSION
        }
    };

    printf("Hello, %s from mod_hello2!\n", host->name);

    return &script;
}

IMPORTABLE("hello2")
