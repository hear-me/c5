#include "import.h"
#include "script.h"

#include <stdbool.h>
#include <stdio.h>

#include "mod/glfw.h"
#include "mod/hello2.h"

const char * HOSTNAME = "main";
const int HOSTVER[3] = { 1, 0, 0 };

__attribute__((destructor)) static void cleanup() {
    importable_unload_all();
}

int main(int argc, char ** argv) {

    const struct script * hello2_script = import("hello2");
//    void (*ext)() = script_ext_get(hello2_script, "foo")->data;
//    ext();
    MOD_HELLO2_EXT_FOO_PFN(pfn) = script_ext_get(hello2_script, MOD_HELLO2_EXT_FOO)->data;

    pfn();

//    script_static_event(hello2_script, 0);

    const struct script * glfw_script = import("glfw");

    /*
    unsigned int count;
    const char ** extensions;
    script_static_event(glfw_script, MOD_GLFW_STATIC_EVENT_GET_REQUIRED_INSTANCE_EXTENSIONS, &count, &extensions);

    for (unsigned int i = 0; i < count; i++) {
        printf("require: %s\n", extensions[i]);
    }
    */

    /*
    const char ** (*glfw_ext)(unsigned int *) =
        script_ext_get(glfw_script, "GetRequiredInstanceExtensions")->data;

    unsigned int count;
    const char ** req = glfw_ext(&count);

    for (unsigned int i = 0; i < count; i++) {
        printf("require: %s\n", req[i]);
    }
    */

    __attribute__((unused)) const struct script * vk_core_script = import("vulkan_core");

    void * glfw_inst = script_create(
            glfw_script,
            800,
            600,
            "Test",
            false
        );

    bool should_exit = false;

    while (!should_exit) {
        script_event(glfw_script, glfw_inst, MOD_GLFW_EVENT_POLL, &should_exit);
    }

    MOD_GLFW_EXT_TIMER_GET_PFN(get_time) = script_ext_get(glfw_script, MOD_GLFW_EXT_TIMER_GET)->data;

    printf("get_time() = %g\n", get_time());

    script_destroy(glfw_script, glfw_inst);
}

