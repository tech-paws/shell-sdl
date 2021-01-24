#pragma once

#include "memory.hpp"
#include "transforms.hpp"
#include "gapi.hpp"
#include "shell_memory.hpp"
#include "shell_config.hpp"
#include "vm.hpp"

struct FrameInfo {
    float current_time = 0.f;
    float last_time = 0.f;
    float delta_time = 0.f;
    float frame_time = 0.f;
    int frames = 0;
    int fps = 0;
};

struct ShellState {
    ShellConfig config;
    FrameInfo frame_info;
    ShellMemory memory;

    Transforms2D test_sprite_transforms;
    glm::mat4 test_sprite_model_matrix;
    glm::mat4 test_sprite_mvp_matrix;
    Texture2D test_sprite_texture;

    CameraMatrices camera_matrices;
    OthroCameraTransforms camera_transform = {
        .viewport_size = vm_vec2f(1024, 768),
        .position = vm_vec2f(0, 0),
        .zoom = 1.f
    };
};
