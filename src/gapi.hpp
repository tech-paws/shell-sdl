#pragma once

#include "primitives.hpp"
#include "platform.hpp"
#include "shell_memory.hpp"
#include "assets.hpp"
#include "shell_config.hpp"

struct GApi;

struct GApiContext;

#ifdef GAPI_OPENGL

#include "gapi/opengl.hpp"

    #ifdef PLATFORM_SDL2

    #include "gapi/opengl_sdl2.hpp"

    #endif

#endif

struct Texture2D;

struct Texture2DParameters {
    bool wrap_s;
    bool wrap_t;
    bool min_filter;
    bool mag_filter;
};

Result<GApi> gapi_init(ShellConfig const& config);

Result<GApiContext> gapi_create_context(Platform& platform, Window window);

void gapi_swap_window(Platform& platform, Window window);

void gapi_shutdown(GApiContext context);

void gapi_clear(float r, float g, float b);

void gapi_render(GApi& gapi);

Texture2D gapi_create_texture_2d(AssetData data, Texture2DParameters params);

void gapi_delete_texture_2d(Texture2D texture);

void gapi_set_viewport(int x, int y, int width, int height);
