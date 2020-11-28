#include "platform/sdl2.hpp"

Result<Platform> platformInit() {
    // Init
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return resultCreateGeneralError<Platform>(
            ErrorCode::PlatformInit,
            SDL_GetError()
        );
    }

    auto initGapiResult = gapiInit();

    if (resultHasError(initGapiResult)) {
        return switchError<Platform>(initGapiResult);
    }

    Platform platform = {
        .gapi = resultGetPayload(initGapiResult)
    };

    return resultCreateSuccess(platform);
}

Result<Window> platformCreateWindow(Platform platform) {
    auto sdlWindow = SDL_CreateWindow(
        "Game", // TODO(sysint64): rm hardcode
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1024, // TODO(sysint64): rm hardcode
        768, // TODO(sysint64): rm hardcode
        SDL_WINDOW_OPENGL
    );

    if (sdlWindow == nullptr) {
        return resultCreateGeneralError<Window>(
            ErrorCode::CreateWindow,
            SDL_GetError()
        );
    }

    Window window = {
        .sdlWindow = sdlWindow
    };

    auto createContextResult = gapiCreateContext(platform, window);

    if (resultHasError(createContextResult)) {
        return switchError<Window>(createContextResult);
    }

    window.gapiContext = resultGetPayload(createContextResult);

    return resultCreateSuccess(window);
}

bool platformEventLoop(Platform platform, Window window) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
    }

    return true;
}

float platformGetTicks() {
    return SDL_GetTicks();
}

void platformSwapWindow(Platform platform, Window window) {
    SDL_GL_SwapWindow(window.sdlWindow);
}

void platformShutdown(Platform platform) {
    SDL_Quit();
}

void platformDestroyWindow(Window window) {
    gapiShutdown(window.gapiContext);
    SDL_DestroyWindow(window.sdlWindow);
}
