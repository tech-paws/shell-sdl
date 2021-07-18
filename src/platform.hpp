#pragma once

#include <initializer_list>
#include <functional>
#include "shell_config.hpp"
#include "assets.hpp"
#include "vm_math.hpp"

struct Platform;

struct Window;

#ifdef PLATFORM_SDL2

#include "platform/sdl2.hpp"

#endif

#include "primitives.hpp"

struct Font;

Result<Platform> platform_init();

extern "C" Vec2f platform_get_mouse_state();

Result<Window> platform_create_window(ShellConfig const& config, Platform& platform);

Result<AssetData> platform_load_font(ShellConfig const& config, RegionMemoryBuffer* dest_memory, const char* asset_name);

bool platform_event_loop(Platform& platform, Window& window);

void platform_get_window_size(Window& window, int* width, int* height);

float platform_get_ticks();

void platform_swap_window(Platform& platform, Window& window);

void platform_destroy_window(Window& window);

void platform_shutdown(Platform& platform);

u8* platform_alloc(MemoryIndex size);

const char platform_preffered_path_separator =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

template<typename... Args>
void platform_build_path(char* dst, Args... args) {
    size_t offset = 0;

    for(const auto arg : {args...}) {
        strcpy(&dst[offset], arg);
        offset += strlen(arg) + 1; // NOTE(sysint64): +1 for separator
        dst[offset - 1] = platform_preffered_path_separator;
    }

    if (offset > 0) {
        dst[offset - 1] = 0;
    }
}
