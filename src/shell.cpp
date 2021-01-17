#include "shell.hpp"
#include "assets.hpp"

static void shell_render(Platform& platform, ShellState& shell_state);

static void shell_step(ShellState& shell_state, double deltaTime);

static void init_texture(ShellState& shell_state) {
    const Result<AssetData> test_texture_result = asset_load_data(
        &shell_state.memory.assets_buffer,
        AssetType::texture,
        "test.jpg"
    );

    const AssetData test_texture = result_unwrap(test_texture_result);
    const Texture2DParameters params = {
        .min_filter = true,
        .mag_filter = true,
    };
    shell_state.test_sprite_texture = gapi_create_texture_2d(test_texture, params);
}

Result<ShellState> shell_init() {
    auto shell_state = ShellState();

    auto buffer_result = create_region_memory_buffer(megabytes(85));

    if (result_is_success(buffer_result)) {
        auto memory_root_buffer = result_get_payload(buffer_result);

        region_memory_buffer_add_region(&memory_root_buffer, &shell_state.memory.assets_buffer, megabytes(40));
        region_memory_buffer_add_region(&memory_root_buffer, &shell_state.memory.frame_buffer, megabytes(10));

        init_texture(shell_state);

        return result_create_success(shell_state);
    } else {
        return switch_error<ShellState>(buffer_result);
    }
}

void shell_main_loop(ShellState& shell_state, Platform& platform, Window window) {
    FrameInfo& frame_info = shell_state.frame_info;
    frame_info.current_time = platform_get_ticks();
    frame_info.delta_time = frame_info.last_time - frame_info.current_time;

    if (frame_info.current_time >= frame_info.last_time + PART_TIME) {
        shell_step(shell_state, frame_info.delta_time);
        shell_render(platform, shell_state);
        frame_info.frames += 1;
        frame_info.last_time = frame_info.current_time;
        gapi_swap_window(platform, window);
    }

    if (frame_info.current_time >= frame_info.frame_time + 1000.0) {
        frame_info.frame_time = frame_info.current_time;
        frame_info.fps = frame_info.frames;
        frame_info.frames = 1;

        printf("FPS: %d\n", frame_info.fps);
    }
}

static void shell_render(Platform& platform, ShellState& shell_state) {
    gapi_clear(0.0f, 0.0f, 0.0f);
    // gapi_render2(platform.gapi);
}

static void shell_step(ShellState& shell_state, double delta_time) {
}

void shell_shutdown() {
}
