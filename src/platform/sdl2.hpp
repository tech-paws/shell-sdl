#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gapi.hpp"
#include <vector>

struct Platform {
    GApi gapi;
};

struct Window {
    SDL_Window* sdl_window;
    GApiContext gapi_context;
};

struct FontSize {
    u64 size;
    TTF_Font* font;
};

struct Font {
    char name[1024];
    char path[1024];
    char relative_path[1024];
    size_t sizes_count;
    FontSize sizes[1024];
};
