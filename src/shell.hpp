#pragma once

#include "platform.hpp"
#include "primitives.hpp"
#include "memory.hpp"
#include "shell_state.hpp"

static const float PART_TIME = 1000.f / 60.f;

Result<ShellState> shellInit();

void shellMainLoop(ShellState& shellState, Platform& platform, Window window);

void shellShutdown();
