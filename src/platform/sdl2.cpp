#include "platform/sdl2.hpp"

Result<Platform> platform_init() {
    // Init
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return result_create_general_error<Platform>(
            ErrorCode::PlatformInit,
            SDL_GetError()
        );
    }

    return result_create_success(Platform());
}

Result<Window> platform_create_window(Platform& platform) {
    auto sdl_window = SDL_CreateWindow(
        "Game", // TODO(sysint64): rm hardcode
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1024, // TODO(sysint64): rm hardcode
        768, // TODO(sysint64): rm hardcode
        SDL_WINDOW_OPENGL
    );

    if (sdl_window == nullptr) {
        return result_create_general_error<Window>(
            ErrorCode::CreateWindow,
            SDL_GetError()
        );
    }

    Window window = {
        .sdl_window = sdl_window
    };

    auto create_context_result = gapi_create_context(platform, window);

    if (result_has_error(create_context_result)) {
        return switch_error<Window>(create_context_result);
    }

    window.gapi_context = result_get_payload(create_context_result);

    auto init_gapi_result = gapi_init();

    if (result_has_error(init_gapi_result)) {
        return switch_error<Window>(init_gapi_result);
    }

    platform.gapi = result_get_payload(init_gapi_result);
    return result_create_success(window);
}

bool platform_event_loop(Platform& platform, Window& window) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
    }

    return true;
}

float platform_get_ticks() {
    return SDL_GetTicks();
}

void platform_swap_window(Platform& platform, Window& window) {
    SDL_GL_SwapWindow(window.sdl_window);
}

void platform_shutdown(Platform& platform) {
    SDL_Quit();
}

void platform_destroy_window(Window& window) {
    gapi_shutdown(window.gapi_context);
    SDL_DestroyWindow(window.sdl_window);
}
