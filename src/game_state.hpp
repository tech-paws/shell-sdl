#pragma once

#include "memory.hpp"
#include "transforms.hpp"

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

    Transforms2D testSpriteTransforms;
    glm::mat4 testSpriteModelMatrix;
    glm::mat4 testSpriteMVPMatrix;

    CameraMatrices cameraMatrices;
    OthroCameraTransforms cameraTransform = {
        .viewportSize = glm::vec2(1024, 768),
        .position = glm::vec2(0, 0),
        .zoom = 1.f
    };
};
