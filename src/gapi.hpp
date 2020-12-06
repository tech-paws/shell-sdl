#pragma once

#include "primitives.hpp"
#include "platform.hpp"
#include "game_memory.hpp"
#include "assets.hpp"

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
    bool wrapS;
    bool wrapT;
    bool minFilter;
    bool magFilter;
};

Result<GApi> gapiInit();

Result<GApiContext> gapiCreateContext(Platform& platform, Window window);

void gapiSwapWindow(Platform& platform, Window window);

void gapiShutdown(GApiContext context);

void gapiClear(float r, float g, float b);

void gapiRender(GApi& gapi, GameMemory& memory);

void gapiRender2(GApi& gapi);

Texture2D gapiCreateTexture2D(AssetData data, Texture2DParameters params);

void gapiDeleteTexture2D(Texture2D texture);
