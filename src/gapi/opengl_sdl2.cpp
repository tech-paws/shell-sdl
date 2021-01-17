#include "gapi/opengl_sdl2.hpp"
#include "platform.hpp"

Result<GApiContext> gapi_create_context(Platform& platform, Window window) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);

    auto gl_context = SDL_GL_CreateContext(window.sdl_window);

    if (gl_context == nullptr) {
        return result_create_general_error<GApiContext>(
            ErrorCode::GApiCreateContext,
            SDL_GetError()
        );
    }

    SDL_GL_MakeCurrent(window.sdl_window, gl_context);
    SDL_GL_SwapWindow(window.sdl_window);

    if (glewInit() != GLEW_OK) {
        return result_create_general_error<GApiContext>(
            ErrorCode::GApiInit,
            "glewInit() error"
        );
    }

    GApiContext gapiContext = {
        .gl_context = gl_context
    };

    return result_create_success(gapiContext);
}

void gapi_swap_window(Platform& platform, Window window) {
    SDL_GL_SwapWindow(window.sdl_window);
}

void gapi_shutdown(GApiContext context) {
    SDL_GL_DeleteContext(context.gl_context);
}
