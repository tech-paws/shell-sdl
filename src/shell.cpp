#include "shell.hpp"
#include "assets.hpp"
#include "vm.hpp"
#include "shell_config.hpp"

static void shell_render(Platform& platform, ShellState& shell_state);

static bool shell_step(ShellState& shell_state, double deltaTime);

Result<ShellState> shell_init(ShellConfig const& config) {
    auto shell_state = ShellState();
    shell_state.config = config;

    auto buffer_result = create_region_memory_buffer(megabytes(85));

    if (result_is_success(buffer_result)) {
        auto memory_root_buffer = result_get_payload(buffer_result);

        region_memory_buffer_add_region(&memory_root_buffer, &shell_state.memory.assets_buffer, megabytes(40));
        region_memory_buffer_add_region(&memory_root_buffer, &shell_state.memory.frame_buffer, megabytes(10));

        return result_create_success(shell_state);
    } else {
        return switch_error<ShellState>(buffer_result);
    }
}

void shell_main_loop(ShellState& shell_state, Platform& platform, Window window) {
    FrameInfo& frame_info = shell_state.frame_info;
    frame_info.current_time = platform_get_ticks();
    frame_info.delta_time = frame_info.last_time - frame_info.current_time;

    if (shell_step(shell_state, frame_info.delta_time) || !shell_state.rendered) {
        tech_paws_vm_process_render_commands();
        collect_text_bounds(platform.gapi);

        // tech_paws_vm_flush();
        frame_info.frames += 1;

        if (frame_info.current_time >= frame_info.frame_time + 1000.0) {
            frame_info.frame_time = frame_info.current_time;
            frame_info.fps = frame_info.frames;
            frame_info.frames = 1;

            printf("FPS: %d\n", frame_info.fps);
        }
    }

    shell_render(platform, shell_state);
    shell_state.rendered = true;
    gapi_swap_window(platform, window);
    frame_info.last_time = frame_info.current_time;
}

static void shell_render(Platform& platform, ShellState& shell_state) {
    gapi_clear(0.0f, 0.0f, 0.0f);
    gapi_render(platform.gapi);
}

static bool shell_step(ShellState& shell_state, double delta_time) {
    return tech_paws_vm_process_commands();
}

void shell_shutdown() {
}
