#pragma once

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
