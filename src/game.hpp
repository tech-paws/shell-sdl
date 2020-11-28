#pragma once

#include "platform.hpp"
#include "primitives.hpp"
#include "memory.hpp"
#include "game_state.hpp"

static const float PART_TIME = 1000.f / 60.f;

Result<GameState> gameInit();

void gameMainLoop(GameState& gameState, Platform& platform, Window window);

void gameShutdown();

#include "gapi/commands.hpp"
