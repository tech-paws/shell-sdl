#pragma once

#include "primitives.hpp"
#include "platform.hpp"
#include "gapi/commands.hpp"

struct GApi;

struct GApiContext;

#ifdef GAPI_OPENGL

#include "gapi/opengl.hpp"

    #ifdef PLATFORM_SDL2

    #include "gapi/opengl_sdl2.hpp"

    #endif

#endif

Result<GApi> gapiInit();

Result<GApiContext> gapiCreateContext(Platform platform, Window window);

void gapiSwapWindow(Platform platform, Window window);

void gapiShutdown(GApiContext context);

void gapiClear(float r, float g, float b);
