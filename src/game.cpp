#include "game.hpp"
#include "assets.hpp"

static void game_render(Platform& platform, GameState& game_state);

static void game_step(GameState& game_state, double deltaTime);

static void init_texture(GameState& game_state) {
    const Result<AssetData> test_texture_result = asset_load_data(
        &game_state.memory.assets_buffer,
        AssetType::texture,
        "test.jpg"
    );

    const AssetData testTexture = result_unwrap(test_texture_result);
    const Texture2DParameters params = {
        .min_filter = true,
        .mag_filter = true,
    };
    game_state.test_sprite_texture = gapi_create_texture_2d(testTexture, params);
}

Result<GameState> game_init() {
    auto game_state = GameState();

    auto buffer_result = create_region_memory_buffer(megabytes(85));

    if (result_is_success(buffer_result)) {
        auto memory_root_buffer = result_get_payload(buffer_result);

        region_memory_buffer_add_region(&memory_root_buffer, &game_state.memory.gapi_commands_buffer, megabytes(10));
        region_memory_buffer_add_region(&memory_root_buffer, &game_state.memory.gapi_commands_data_buffer, megabytes(10));
        region_memory_buffer_add_region(&memory_root_buffer, &game_state.memory.assets_buffer, megabytes(40));
        region_memory_buffer_add_region(&memory_root_buffer, &game_state.memory.frame_buffer, megabytes(10));

        init_texture(game_state);

        return result_create_success(game_state);
    } else {
        return switch_error<GameState>(buffer_result);
    }
}

void game_main_loop(GameState& game_state, Platform& platform, Window window) {
    FrameInfo& frame_info = game_state.frame_info;
    frame_info.current_time = platform_get_ticks();
    frame_info.delta_time = frame_info.last_time - frame_info.current_time;

    if (frame_info.current_time >= frame_info.last_time + PART_TIME) {
        game_step(game_state, frame_info.delta_time);
        game_render(platform, game_state);
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

static void game_render(Platform& platform, GameState& game_state) {
    gapi_clear(0.0f, 0.0f, 0.0f);

    // gapi_set_color_pipeline(game_state.memory, glm::vec4(1, 0, 0, 1));
    gapi_set_texture_pipeline(game_state.memory, game_state.test_sprite_texture.id);

    draw_centered_quads(game_state.memory, &game_state.test_sprite_mvp_matrix, 1);
    gapi_render(platform.gapi, game_state.memory);
}

static void game_step(GameState& game_state, double delta_time) {
    game_state.camera_matrices = transforms_create_ortho_camera_matrices(game_state.camera_transforms);

    game_state.test_sprite_transforms.position = glm::vec2(
        game_state.camera_transforms.viewport_size.x / 2.f,
        game_state.camera_transforms.viewport_size.y / 2.f
    );
    game_state.test_sprite_transforms.scaling = glm::vec2(430.0f, 600.0f);
    game_state.test_sprite_transforms.rotation += (0.25f/1000.f) * delta_time;

    game_state.test_sprite_model_matrix = transforms_create_2d_model_matrix(game_state.test_sprite_transforms);
    game_state.test_sprite_mvp_matrix = game_state.camera_matrices.mvp_matrix * game_state.test_sprite_model_matrix;
}

void gameShutdown() {
}
