#include "common.h"
#include "import.h"

#include <stdlib.h>
#include <string.h>

#ifdef DEBUG_IMPORTS
#include <stdio.h>
#endif /* DEBUG_IMPORTS */

static void importable_register(const char * name, const struct script * (*importer)());

static struct host _host = (struct host) {
    .name = NULL,
    .importable_register = &importable_register
};

struct host * host = NULL;

__attribute__((constructor(1000))) static void init() {
    _host.name = HOSTNAME;
    host = &_host;
}

static unsigned int importable_module_count = 0;
static struct importable * importable_modules = NULL;

unsigned int importable_count_get() {
    return importable_module_count;
}

struct importable * importable_get_all() {
    return importable_modules;
}

static void importable_register(const char * name, const struct script * (*importer)()) {

#ifdef DEBUG_IMPORTS
    for (unsigned int i = 0; i < importable_module_count; i++) {
        if (strcmp(name, importable_modules[i].name) == 0) {
            fprintf(stderr, "DEBUG_IMPORTS: repeated module name '%s'\n", name);
        }
    }
#endif

    struct importable * p = malloc(sizeof(*importable_modules) * (importable_module_count + 1));
    struct importable * pold = importable_modules;
    memmove(p, pold, sizeof(*pold) * (importable_module_count));
    importable_modules = p;
    free(pold);

    importable_modules[importable_module_count] = (struct importable) {
        .name = name,
        .importer = importer
    };
    importable_module_count++;
}

const struct script * importable_import(struct importable * importable) {
    if (!(importable->imported)) {
        importable->imported = importable->importer();
    }
    return importable->imported;
}

void importable_unload(struct importable * importable) {
    if (importable->imported) {
        script_unload(importable->imported);
        importable->imported = NULL;
    }
}

void importable_unload_all() {
    for (unsigned int i = 0; i < importable_module_count; i++) {
        importable_unload(&importable_modules[i]);
    }
}

const struct script * import(const char * name) {
    unsigned int count = importable_count_get();
    struct importable * importables = importable_get_all();
    for (unsigned int i = 0; i < count; i++) {
        if (strcmp(importable_get_name(&importables[i]), name) == 0) {
            return importable_import(&importables[i]);
        }
    }
#ifdef DEBUG_IMPORTS
    fprintf(stderr, "DEBUG_IMPORTS: import(%s) not found\n", name);
#endif /* DEBUG_IMPORTS */
    return NULL;
}

