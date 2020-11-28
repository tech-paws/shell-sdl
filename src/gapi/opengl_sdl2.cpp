#include "gapi/opengl_sdl2.hpp"
#include "platform.hpp"

Result<GApiContext> gapiCreateContext(Platform& platform, Window window) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);

    auto glContext = SDL_GL_CreateContext(window.sdlWindow);

    if (glContext == nullptr) {
        return resultCreateGeneralError<GApiContext>(
            ErrorCode::GApiCreateContext,
            SDL_GetError()
        );
    }

    SDL_GL_MakeCurrent(window.sdlWindow, glContext);
    SDL_GL_SwapWindow(window.sdlWindow);

    if (glewInit() != GLEW_OK) {
        return resultCreateGeneralError<GApiContext>(
            ErrorCode::GApiInit,
            "glewInit() error"
        );
    }

    GApiContext gapiContext = {
        .glContext = glContext
    };

    return resultCreateSuccess(gapiContext);
}

void gapiSwapWindow(Platform& platform, Window window) {
    SDL_GL_SwapWindow(window.sdlWindow);
}

void gapiShutdown(GApiContext context) {
    SDL_GL_DeleteContext(context.glContext);
}
