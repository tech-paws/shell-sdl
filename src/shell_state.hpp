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
};
