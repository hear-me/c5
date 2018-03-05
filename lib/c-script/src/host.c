#include "common.h"
#include "import.h"

#include <stdlib.h>
#include <string.h>

#ifdef DEBUG_SCRIPT
#include <stdio.h>
#endif /* DEBUG_SCRIPT */

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

#ifdef DEBUG_SCRIPT
    for (unsigned int i = 0; i < importable_module_count; i++) {
        if (strcmp(name, importable_modules[i].name) == 0) {
            fprintf(stderr, "DEBUG_IMPORTS: repeated module name '%s'\n", name);
        }
    }
#endif /* DEBUG_SCRIPT */

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
    struct importable * importable = importable_get(name);
    if (importable) {
        return importable_import(importable);
    } else {
        return NULL;
    }
}

struct importable * importable_get(const char * name) {
    unsigned int count = importable_count_get();
    struct importable * importables = importable_get_all();
    for (unsigned int i = 0; i < count; i++) {
        if (strcmp(importables[i].name, name) == 0) {
            return &importables[i];
        }
    }
#ifdef DEBUG_SCRIPT
    fprintf(stderr, "DEBUG_IMPORTS: import(%s) not found\n", name);
#endif /* DEBUG_SCRIPT */
    return NULL;
}

