#ifndef SCRIPT_SCRIPT_H
#define SCRIPT_SCRIPT_H

#include <stdarg.h>
#include <stdlib.h>

struct extension {
    int id;
    const char * name;
    void * data;
};

struct script {
    void (*unload)(struct script * s);
    void * (*create)(struct script *, va_list);
    void (*destroy)(struct script *, void *);
    void (*static_event)(struct script *, int, va_list);
    void (*event)(struct script *, void *, int, va_list);
    struct extension extensions[];
};

void script_unload(const struct script * s);
void * script_create(const struct script * s, ...);
void script_destroy(const struct script * s, void * inst);
void script_static_event(const struct script * s, int ev, ...);
void script_event(const struct script * s, void * inst, int ev, ...);
const struct extension * script_ext_get(const struct script * s, int id);

#endif /* SCRIPT_SCRIPT_H */
