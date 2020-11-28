#include "game.hpp"

static void gameRender();

Result<GameState> gameInit() {
    auto gameState = GameState();
    return resultCreateSuccess(gameState);
}

void gameMainLoop(GameState& gameState, Platform platform, Window window) {
    FrameInfo& frameInfo = gameState.frameInfo;
    frameInfo.currentTime = platformGetTicks();
    frameInfo.deltaTime = frameInfo.lastTime - frameInfo.currentTime;

    if (frameInfo.currentTime >= frameInfo.lastTime + PART_TIME) {
        gameRender();
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

static void gameRender() {
    gapiClear(0.0f, 0.0f, 0.0f);
}

void gameShutdown() {
}
