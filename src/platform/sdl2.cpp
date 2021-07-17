#include "platform/sdl2.hpp"
#include "vm.hpp"
#include "vm_math.hpp"
#include "vm_glm_adapter.hpp"

Result<Platform> platform_init() {
    // Init
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return result_create_general_error<Platform>(
            ErrorCode::PlatformInit,
            SDL_GetError()
        );
    }

    if (TTF_Init() < 0) {
        return result_create_general_error<Platform>(
            ErrorCode::PlatformInit,
            TTF_GetError()
        );
    }

    return result_create_success(Platform());
}

Result<Window> platform_create_window(ShellConfig const& config, Platform& platform) {
    auto sdl_window = SDL_CreateWindow(
        config.window_title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        config.window_width,
        config.window_height,
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

    auto init_gapi_result = gapi_init(config);

    if (result_has_error(init_gapi_result)) {
        return switch_error<Window>(init_gapi_result);
    }

    platform.gapi = result_get_payload(init_gapi_result);
    return result_create_success(window);
}

u8 serialize_mouse_button(int32_t button) {
    switch (button) {
        case SDL_BUTTON_LEFT:
            return COMMAND_MOUSE_BUTTON_LEFT;
        case SDL_BUTTON_RIGHT:
            return COMMAND_MOUSE_BUTTON_RIGHT;
        case SDL_BUTTON_MIDDLE:
            return COMMAND_MOUSE_BUTTON_MIDDLE;
        default:
            return COMMAND_MOUSE_BUTTON_UNKNOWN;
    }
}

bool platform_event_loop(Platform& platform, Window& window) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            const auto bytes_writer = tech_paws_begin_command("tech.paws.client", Source::Processor, COMMAND_TOUCH_START);

            vm_buffers_bytes_writer_write_byte(bytes_writer, serialize_mouse_button(event.button.button));
            vm_buffers_bytes_writer_write_int32_t(bytes_writer, event.button.x);
            vm_buffers_bytes_writer_write_int32_t(bytes_writer, event.button.y);

            tech_paws_end_command("tech.paws.client", Source::Processor);
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            const auto bytes_writer = tech_paws_begin_command("tech.paws.client", Source::Processor, COMMAND_TOUCH_END);

            vm_buffers_bytes_writer_write_byte(bytes_writer, serialize_mouse_button(event.button.button));
            vm_buffers_bytes_writer_write_int32_t(bytes_writer, event.button.x);
            vm_buffers_bytes_writer_write_int32_t(bytes_writer, event.button.y);

            tech_paws_end_command("tech.paws.client", Source::Processor);
        }
        else if (event.type == SDL_MOUSEMOTION) {
            const auto bytes_writer = tech_paws_begin_command("tech.paws.client", Source::Processor, COMMAND_TOUCH_MOVE);

            vm_buffers_bytes_writer_write_int32_t(bytes_writer, event.motion.x);
            vm_buffers_bytes_writer_write_int32_t(bytes_writer, event.motion.y);

            tech_paws_end_command("tech.paws.client", Source::Processor);
        }
    }

    return true;
}

extern "C" Vec2f platform_get_mouse_state() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return vm_vec2f(x, y);
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

Result<AssetData> platform_load_font(ShellConfig const& config, RegionMemoryBuffer* dest_memory, const char* asset_name) {
    char path[1024] { 0 };
    char relative_path[1024] { 0 };

    platform_build_path(&path[0], config.assets_path, "fonts", asset_name);
    platform_build_path(&relative_path[0], "fonts", asset_name);

    FILE* file = fopen(&path[0], "rb");

    if (file == nullptr) {
        return result_create_general_error<AssetData>(
            ErrorCode::LoadAsset,
            "Can't open asset: %s", &relative_path[0]
        );
    }

    fclose(file);

    Font font = {
        .sizes_count = 0,
    };

    strcpy(&font.name[0], asset_name);
    strcpy(&font.path[0], &path[0]);
    strcpy(&font.relative_path[0], &relative_path[0]);

    Result<u8*> data_result = region_memory_buffer_alloc(dest_memory, sizeof(Font));

    if (result_has_error(data_result)) {
        return switch_error<AssetData>(data_result);
    }

    u8* data = result_get_payload(data_result);
    memcpy(data, &font, sizeof(Font));

    AssetData asset_data = {
        .size = sizeof(Font),
        .data = data
    };

    log_info("Successfully loaded asset: %s", relative_path);
    return result_create_success(asset_data);
}

Result<TTF_Font*> get_sdl2_ttf_font(Font* font, u32 font_size) {
    for (size_t i = 0; i < font->sizes_count; i += 1) {
        if (font->sizes[i].size == font_size) {
            return result_create_success(font->sizes[i].font);
        }
    }

    auto ttf_font = TTF_OpenFont(&font->path[0], font_size);

    if (!ttf_font) {
        return result_create_general_error<TTF_Font*>(
            ErrorCode::GetTTFFont,
            TTF_GetError()
        );
    }

    font->sizes_count += 1;
    assert(font->sizes_count < 1024);

    font->sizes[font->sizes_count - 1].size = font_size;
    font->sizes[font->sizes_count - 1].font = ttf_font;

    return result_create_success(font->sizes[font->sizes_count - 1].font);
}
