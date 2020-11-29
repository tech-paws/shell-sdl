#pragma once

#include "memory.hpp"

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
    RegionMemoryBuffer gapiCommandsDataBuffer;
    RegionMemoryBuffer rootBuffer;
    RegionMemoryBuffer assetsBuffer;
    RegionMemoryBuffer frameBuffer;
};

struct GameState {
    FrameInfo frameInfo;
    GameMemory memory;
};
