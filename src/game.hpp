#pragma once

#include "platform.hpp"
#include "primitives.hpp"
#include "memory.hpp"
#include "game_state.hpp"

static const float PART_TIME = 1000.f / 60.f;

Result<GameState> game_init();

void game_main_loop(GameState& game_state, Platform& platform, Window window);

void game_shutdown();

#include "gapi/commands.hpp"
