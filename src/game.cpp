#include "game.hpp"

static void gameRender(Platform& platform, GameState& gameState);

Result<GameState> gameInit() {
    auto gameState = GameState();

    auto bufferResult = createRegionMemoryBuffer(megabytes(85));

    if (resultIsSuccess(bufferResult)) {
        auto memoryRootBuffer = resultGetPayload(bufferResult);

        regionMemoryBufferAddRegion(&memoryRootBuffer, &gameState.memory.gapiCommandsBuffer, megabytes(10));
        regionMemoryBufferAddRegion(&memoryRootBuffer, &gameState.memory.gapiCommandsDataBuffer, megabytes(10));
        regionMemoryBufferAddRegion(&memoryRootBuffer, &gameState.memory.assetsBuffer, megabytes(40));
        regionMemoryBufferAddRegion(&memoryRootBuffer, &gameState.memory.frameBuffer, megabytes(10));

        return resultCreateSuccess(gameState);
    } else {
        return switchError<GameState>(bufferResult);
    }

    return resultCreateSuccess(gameState);
}

void gameMainLoop(GameState& gameState, Platform& platform, Window window) {
    FrameInfo& frameInfo = gameState.frameInfo;
    frameInfo.currentTime = platformGetTicks();
    frameInfo.deltaTime = frameInfo.lastTime - frameInfo.currentTime;

    if (frameInfo.currentTime >= frameInfo.lastTime + PART_TIME) {
        gameRender(platform, gameState);
        frameInfo.frames += 1;
        frameInfo.lastTime = frameInfo.currentTime;
        gapiSwapWindow(platform, window);
    }

    if (frameInfo.currentTime >= frameInfo.frameTime + 1000.0) {
        frameInfo.frameTime = frameInfo.currentTime;
        frameInfo.fps = frameInfo.frames;
        frameInfo.frames = 1;

        printf("FPS: %d\n", frameInfo.fps);
    }
}

static void gameRender(Platform& platform, GameState& gameState) {
    gapiClear(0.0f, 0.0f, 0.0f);

    gapiSetColorPipeline(gameState, glm::vec4(1, 0, 0, 1));

    auto quad = glm::vec4(0, 0, 100, 100);
    drawQuads(gameState, &quad, 1);

    gapiRender(platform.gapi, gameState);
}

void gameShutdown() {
}
