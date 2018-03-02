#ifndef SCRIPT_IMPORT_H
#define SCRIPT_IMPORT_H

#include "script.h"

struct importable {
    const char * name;
    const struct script * (*importer)();
    const struct script * imported;
};

unsigned int importable_count_get();
struct importable * importable_get_all();
void importable_unload(struct importable * importable);
void importable_unload_all();
const struct script * importable_import(struct importable * importable);

// lazy import
const struct script * import(const char * name);

static inline const char * importable_get_name(struct importable * importable) {
    return importable->name;
}

#endif /* SCRIPT_IMPORT_H */
