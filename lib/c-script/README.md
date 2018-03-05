C-Script
========

This library provides the module infrastructure used in this project.

Modules (loosely "scripts") are self-registering psuedo-libraries.
The basic concept is a replacement for `dlopen()` functionality as
it is used with shared libraries to modularize code.
The modules in this library, however, require no platform-dependent
loading mechanism, as they are all compiled into the resulting program.

Behavior is standardized through a fairly flexible API exposed by each
module and a clean import mechanism to hook things together.

## Hosting

The *host* is the non-module root of the program. It consists of the
bootstrapping code in `src/host.c`.

Programs that include `host.c`
(bundled into the static archive `c-script.a` by the build script in
this directory) automatically set up a module registry before `main` is
invoked using the "constructor" extension to C
(available in both GCC and Clang.)
No initialization calls need be performed before the code can be used,
it "just works."

### API

There is no specific API or header associated with the host, since it
does not interface directly with user code.

The only minor exception to this is that the library contains a weak
symbol `HOSTNAME` that can (and should) be overriden by the user program.

Simply include:
```c
const char * HOSTNAME = "...";
```

## Modules

A *module* is, as previously mentioned, an embedded micro-library
with a well-defined public API.

There are no modules defined directly in `c-script` but they are
needed for it to be useful. Look at the main project for examples.

A given C source file is made a module by following a simple framework:
```c
#include "importable.h"

/* ... */

IMPORTABLE("module_name") // unique name to identify module
```

The `IMPORTABLE` declaration assumes the presence of a function
in the translation unit called `import` that takes no arguments
and returns `const struct script *`. This structure is defined
in `script.h` and implicitly included via `importable.h`.

The `import` function should return a pointer to one of these
structs (typically declared statically in the function itself)
filled appropriately for the module. The returned pointer
is the external access point to the module. Note that `import`
is only called once when the module is first loaded (see below),
subsequent calls return the same pointer without calling the
function again.

### API

The `struct script` is defined (in `script.h`) and contains
five function pointers (set to `NULL` to disable for the module)
and a variable-length array of `struct extension` objects.

The function signatures are:
  - `void unload(struct script * script)`:
    called when the module is unloaded
  - `void * create(struct script * script, va_list args)`:
    called to create an "instance" from the module
    (semantics are module dependent)
  - `void destroy(struct script * script, void * inst)`:
    called to destroy an "instance" from the module
    (should be passed something returned from `create`)
  - `void static_event(struct script * script, int ev, va_list args)`:
    called to trigger an event on the whole module (a la "static method")
  - `void event(struct script * script, void * inst, int ev, va_list args)`:
    called to trigger an event with an "instance" returned from `create`
    (a la "instance method")

Events are identified by an `int`, library users will likely provide
`#define`s or similar to create friendly names.

The `struct extension` type contains:
  - `int id`
  - `const char * name`
  - `void * data`

The `importable.h` header defines a constant `NULL_EXTENSION` used to
terminate the `struct extension` array.

Extensions are queried by `int` similarly to the event API.
The `name` field provides a friendly descriptor of the extension
and serves to identify the terminal value in the array (e.g. queries
terminate upon reaching the first extensions where `!name`.

The `NULL_EXTENSION` is defined with an ID of 0. A query for extension 0
will return this terminator if: no extension with a zero ID
and no terminator (null `name`) extension with a non-zero ID
occurs earlier in the array.

## Importing

Modules are registered automatically but their `import` function
is not called until they are accessed (*imported*) for the first time.

The methods for doing so are declared in `import.h` along with
those for cleanup (*unloading.*)

An API is provided for iterating all registered modules:
  - `unsigned int importable_count_get()`: returns the number
    of registered modules.
  - `struct importable * importable_get(unsigned int index)`: returns
    a pointer to the registered module at that index.
  - `bool importable_get_imported(struct importable * importable)`:
    returns `true` if the module has been imported already (or unloaded
    after an import) and `false` otherwise.
  - The type `struct importable` is not opaque; the following fields
    are accessible:
     - `const char * name`: the module name
     - `const struct script * (*importer)()`: a function pointer
       to the modules `import` function. (Not normally used unless
       caching of the import result must be bypassed.)
     - `const struct script * imported`: the cached import result
       or `NULL` if not imported. (Prefer `importable_get_imported` when
       simply testing `NULL`-ness of this field.

Actual imports are performed in one of two ways:
  - `const struct script * importable_import(struct importable * importable)`: imports the reference (typically during iteration.)
  - `const struct script * import(const char * name)`: searches for
    module by name and imports it or returns `NULL`.
    (`#ifdef DEBUG_IMPORTS`: it will also print a message to stderr if
    the module is not found.)

Unloading is performed with:
  - `void importable_unload(struct importable * importable)`: unloads
    the referenced module.
  - `void importable_unload_all()`: unloads all registered modules.

Note that the API specifies that an import of a module that has
been unloaded will result in the `import` function being called again.
If a module must not support this feature, it must detect the condition
and trigger an error condition or return a modified script pointer.
In the case that it returns a `NULL` value, the module will not be
considered to have been imported; i.e. unloading will not occur
and further imports will continue to call the `import` function.
