#include "mod/glfw.h"
#include "importable.h"
#include "error.h"
#define MODNAME "mod_glfw"

#include <string.h>
#include <stdbool.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/*
#include "import.h"
#include "mod/log.h"
static const struct script * mod_log = NULL;
*/

static GLFWwindow * window = NULL;

static void unload(struct script * script) {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

static void error_callback(int err, const char * description) {
    error("GLFW indicated error %d: %s\n", err, description);
}

/* (int width, int height, const char * title, bool fullscreen) */
static void * create_window(struct script * script, va_list args) {
    int w = va_arg(args, int);
    int h = va_arg(args, int);
    const char * title = va_arg(args, const char *);
    bool fullscreen = va_arg(args, int);

    if (window) {
        return window;
    }

    GLFWmonitor * primary = NULL;

    if (w == 0 || h == 0) {
        if (!fullscreen) {
            error("cannot infer resolution of non-fullscreen window\n");
        }
        primary = glfwGetPrimaryMonitor();
        const GLFWvidmode * mode = glfwGetVideoMode(primary);

        w = mode->width;
        h = mode->height;
    } else if (fullscreen) {
        primary = glfwGetPrimaryMonitor();
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(w, h, title, primary, NULL);

    if (!window) {
        error("failed to create window\n");
    }

    /* TODO create VK_KHR_surface */

    return window;
}

static void destroy_window(struct script * script, void * inst) {
    if (inst && inst != window) {
        error("instance passed for destruction does not match singleton\n");
    }
    glfwDestroyWindow(window);
    window = NULL;
}

static void static_event(struct script * s, int ev, va_list args) {
    unsigned int * count;
    const char *** ext;
    unsigned int index;
    void * v0, * v1;
    bool * out;

    switch (ev) {
        default:
            break;
        case MOD_GLFW_STATIC_EVENT_QUERY_PRESENTATION_SUPPORT:
             v0 = va_arg(args, typeof(v0));
             v1 = va_arg(args, typeof(v1));
             index = va_arg(args, typeof(index));
             out = va_arg(args, typeof(out));
             *out = (bool)glfwGetPhysicalDevicePresentationSupport(v0, v1, index);
             break;
        case MOD_GLFW_STATIC_EVENT_GET_REQUIRED_INSTANCE_EXTENSIONS:
             count = va_arg(args, typeof(count));
             ext = va_arg(args, typeof(ext));
             *ext = glfwGetRequiredInstanceExtensions(count);
             break;
    }
}

static void window_event(struct script * s, void * inst, int ev, va_list args) {
    if (inst && inst != window) {
        error("instance passed for event does not match singleton\n");
    }
    bool * should_close;
    void * cb;
    bool unlimited;

    switch (ev) {
        default:
            break;
        case MOD_GLFW_EVENT_POLL:
            should_close = va_arg(args, bool *);
            if (should_close && glfwWindowShouldClose(window)) {
                *should_close = true;
            } else {
                glfwPollEvents();
            }
            break;
        case MOD_GLFW_EVENT_KEY_CALLBACK:
            cb = va_arg(args, void *);
            glfwSetKeyCallback(window, cb);
            break;
        case MOD_GLFW_EVENT_BUTTON_CALLBACK:
            cb = va_arg(args, void *);
            glfwSetMouseButtonCallback(window, cb);
            break;
        case MOD_GLFW_EVENT_CURSOR_CALLBACK:
            cb = va_arg(args, void *);
            glfwSetCursorPosCallback(window, cb);
            unlimited = va_arg(args, int);
            if (unlimited) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            break;
    }
}

static const struct script * _import() {

    const static struct script script = {
        .unload = &unload,
        .create = &create_window,
        .destroy = &destroy_window,
        .static_event = &static_event,
        .event = &window_event,
        .extensions = {
            {
                .id = MOD_GLFW_EXT_TIMER_GET,
                .name = "glfwGetTime",
                .data = (void*)&glfwGetTime
            },
            {
                .id = MOD_GLFW_EXT_TIMER_GET_VAL,
                .name = "glfwGetTimerValue",
                .data = (void*)&glfwGetTimerValue,
            },
            {
                .id = MOD_GLFW_EXT_TIMER_SET,
                .name = "glfwSetTime",
                .data = (void*)&glfwSetTime
            },
            {
                .id = MOD_GLFW_EXT_GET_INST_PROC_ADDR,
                .name = "glfwGetInstanceProcAddress",
                .data = (void*)glfwGetInstanceProcAddress
            },
            NULL_EXTENSION
        }
    };

    if (!glfwInit()) {
        error("glfwInit() failed\n");
    }

    /*
    mod_log = import("log");
    */

    glfwSetErrorCallback(&error_callback);

    return &script;
}

IMPORTABLE("glfw")

