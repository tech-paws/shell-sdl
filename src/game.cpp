#include "game.hpp"

static void gameRender(Platform& platform, GameState& gameState);

static void gameStep(GameState& gameState, double deltaTime);

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
        gameStep(gameState, frameInfo.deltaTime);
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

    drawCenteredQuads(gameState, &gameState.testSpriteMVPMatrix, 1);
    gapiRender(platform.gapi, gameState);
}

static void gameStep(GameState& gameState, double deltaTime) {
    gameState.cameraMatrices = transformsCreateOrthoCameraMatrices(gameState.cameraTransform);

    gameState.testSpriteTransforms.position = glm::vec2(
        gameState.cameraTransform.viewportSize.x / 2.f,
        gameState.cameraTransform.viewportSize.y / 2.f
    );
    gameState.testSpriteTransforms.scaling = glm::vec2(430.0f, 600.0f);
    gameState.testSpriteTransforms.rotation += (0.25f/1000.f) * deltaTime;

    gameState.testSpriteModelMatrix = transformsCreate2DModelMatrix(gameState.testSpriteTransforms);
    gameState.testSpriteMVPMatrix = gameState.cameraMatrices.mvpMatrix * gameState.testSpriteModelMatrix;
}

void gameShutdown() {
}
