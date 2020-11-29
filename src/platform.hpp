#pragma once

#include <initializer_list>

struct Platform;

struct Window;

#ifdef PLATFORM_SDL2

#include "platform/sdl2.hpp"

#endif

#include "primitives.hpp"

Result<Platform> platformInit();

Result<Window> platformCreateWindow(Platform& platform);

bool platformEventLoop(Platform& platform, Window& window);

float platformGetTicks();

void platformSwapWindow(Platform& platform, Window& window);

void platformDestroyWindow(Window& window);

void platformShutdown(Platform& platform);

u8* platformAlloc(MemoryIndex size);

const char platformPrefferedPathSeparator =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

template<typename... Args>
void platformBuildPath(char* dst, Args... args) {
    size_t offset = 0;

    for(const auto arg : {args...}) {
        strcpy(&dst[offset], arg);
        offset += strlen(arg) + 1; // NOTE(sysint64): +1 for separator
        dst[offset - 1] = platformPrefferedPathSeparator;
    }

    if (offset > 0) {
        dst[offset - 1] = 0;
    }

    puts(&dst[0]);
}
