#include "script.h"

#include <string.h>

void script_unload(const struct script * s) {
    if (s->unload) s->unload((struct script *)s);
}

void * script_create(const struct script * s, ...) {
    va_list ap;
    va_start(ap, s);
    void * ret;
    if (s->create) {
        ret = s->create((struct script *)s, ap);
    } else {
        ret = NULL;
    }
    va_end(ap);
    return ret;
}

void script_destroy(const struct script * s, void * inst) {
    if (s->destroy) s->destroy((struct script *)s, inst);
    else if (inst) free(inst);
}

void script_static_event(const struct script * s, int ev, ...) {
    va_list ap;
    va_start(ap, ev);
    if (s->static_event) {
        s->static_event((struct script *)s, ev, ap);
    }
    va_end(ap);
}

void script_event(const struct script * s, void * inst, int ev, ...) {
    va_list ap;
    va_start(ap, ev);
    if (s->event) {
        s->event((struct script *)s, inst, ev, ap);
    }
    va_end(ap);
}

const struct extension * script_ext_get(const struct script * s, int id) {
    unsigned int i = 0;
    do {
        if (s->extensions[i].id == id) {
            return &s->extensions[i];
        }
    } while (s->extensions[i].name);

    return NULL;
}

