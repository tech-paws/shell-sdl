#pragma once

#include "platform.hpp"
#include "primitives.hpp"
#include "memory.hpp"

static const float PART_TIME = 1000.f / 60.f;

struct FrameInfo {
    float currentTime = 0.f;
    float lastTime = 0.f;
    float deltaTime = 0.f;
    float frameTime = 0.f;
    int frames = 0;
    int fps = 0;
};

struct GameMemory {
    RegionMemoryBuffer gapiCommandsBuffer;
    RegionMemoryBuffer rootBuffer;
    RegionMemoryBuffer assetsBuffer;
    RegionMemoryBuffer frameBuffer;
};

struct GameState {
    FrameInfo frameInfo;
    GameMemory memory;
};

Result<GameState> gameInit();

void gameMainLoop(GameState& gameState, Platform platform, Window window);

void gameShutdown();

#include "gapi/commands.hpp"
