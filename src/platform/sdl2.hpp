#pragma once

#include <SDL2/SDL.h>
#include "gapi.hpp"

struct Platform {
    GApi gapi;
};

struct Window {
    SDL_Window* sdl_window;
    GApiContext gapi_context;
};
