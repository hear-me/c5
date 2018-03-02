`mod_glfw`
==========

`mod_glfw` implements GLFW functionality required
for Vulkan usage:
 - querying required extensions
 - querying physical presentation support
 - creating a window w/ `GLFW_NO_API`
 - creating a Vulkan surface (although not destroying it)

It also handles general GLFW lifetime:
 - initialize/terminate
 - error callback (via `mod_log`)

## API
### Static behavior

Static load:
 - calls `glfwInit()`
 - sets error callback

Static unload:
 - calls `glfwTerminate()`

Static events:
 - `MOD_GLFW_STATIC_EVENT_QUERY_PRESENTATION_SUPPORT`: check before window and surface 

### Instances

The module allows creation of a single instance to represent the window.
Also creates the associated Vulkan surface.

Create:
 - Syntax:
    `(int width, int height, const char * title, bool fullscreen)`
 - If width or height are `0`, fullscreen must be `true` and the
   current resolution of the primary monitor is used.
 - Return value should be considered opaque
 - Calls `MOD_VK_STATIC_EVENT_GLFW_SURFACE` on `mod_vk`

Destroy:
 - Destroys the window (also called automatically at unload)

Events:
 - `MOD_GLFW_EVENT_POLL`: poll for events and check "should close"
   - Syntax: `(bool * should_close)`
 - `MOD_GLFW_EVENT_KEY_CALLBACK`: set key callback
   - Syntax: `(void (*callback)(int key, int scancode, int action, int mods)`
 - `MOD_GLFW_EVENT_BUTTON_CALLBACK`: set button callback
   - Syntax: `(void (*callback)(int button, int action, int mods))`
 - `MOD_GLFW_EVENT_CURSOR_CALLBACK`: set cursor callback and "unlimited" mode
   - Syntax: `(void (*callback)(double xpos, double ypos), bool unlimited)`

### Extensions

Extensions for "fast" access to certain paramaters:
  - `MOD_GLFW_EXT_TIMER_GET`: function to query internal GLFW timer
    - Syntax: `double (*)()`
  - `MOD_GLFW_EXT_TIMER_GET_VAL`: function to query internal GLFW timer (as interger)
    - Syntax: `unsigned long (*)()`
  - `MOD_GLFW_EXT_TIMER_SET`: function to set internal GLFW timer
    - Syntax: `void (*)(double time);`

Note that direct polling of keys, buttons, etc. is not provided; use callbacks.


