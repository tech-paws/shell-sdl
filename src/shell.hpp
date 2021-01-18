#pragma once

#include "platform.hpp"
#include "primitives.hpp"
#include "memory.hpp"
#include "shell_state.hpp"
#include "shell_config.hpp"

static const float PART_TIME = 1000.f / 60.f;

Result<ShellState> shell_init(ShellConfig const& config);

void shell_main_loop(ShellState& shell_state, Platform& platform, Window window);

void shell_shutdown();
