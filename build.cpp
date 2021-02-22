#ifdef PLATFORM_SDL2
#include "src/platform/sdl2.cpp"
#endif

#ifdef GAPI_OPENGL
#include "src/gapi/opengl.cpp"

    #ifdef PLATFORM_SDL2
    #include "src/gapi/opengl_sdl2.cpp"
    #endif

#endif

#ifdef __linux__
#include "src/platform/linux.cpp"
#endif

#include "src/assets.cpp"
#include "src/memory.cpp"
#include "src/shell.cpp"
#include "src/lib.cpp"
