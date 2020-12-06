#include "shell.hpp"
#include "assets.hpp"

static void shellRender(Platform& platform, ShellState& shellState);

static void shellStep(ShellState& shellState, double deltaTime);

static void initTexture(ShellState& shellState) {
    const Result<AssetData> testTextureResult = assetLoadData(
        &shellState.memory.assetsBuffer,
        AssetType::texture,
        "test.jpg"
    );

    const AssetData testTexture = resultUnwrap(testTextureResult);
    const Texture2DParameters params = {
        .minFilter = true,
        .magFilter = true,
    };
    shellState.testSpriteTexture = gapiCreateTexture2D(testTexture, params);
}

Result<ShellState> shellInit() {
    auto shellState = ShellState();

    auto bufferResult = createRegionMemoryBuffer(megabytes(85));

    if (resultIsSuccess(bufferResult)) {
        auto memoryRootBuffer = resultGetPayload(bufferResult);

        regionMemoryBufferAddRegion(&memoryRootBuffer, &shellState.memory.assetsBuffer, megabytes(40));
        regionMemoryBufferAddRegion(&memoryRootBuffer, &shellState.memory.frameBuffer, megabytes(10));

        initTexture(shellState);

        return resultCreateSuccess(shellState);
    } else {
        return switchError<ShellState>(bufferResult);
    }
}

void shellMainLoop(ShellState& shellState, Platform& platform, Window window) {
    FrameInfo& frameInfo = shellState.frameInfo;
    frameInfo.currentTime = platformGetTicks();
    frameInfo.deltaTime = frameInfo.lastTime - frameInfo.currentTime;

    if (frameInfo.currentTime >= frameInfo.lastTime + PART_TIME) {
        shellStep(shellState, frameInfo.deltaTime);
        shellRender(platform, shellState);
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

static void shellRender(Platform& platform, ShellState& shellState) {
    gapiClear(0.0f, 0.0f, 0.0f);
    gapiRender2(platform.gapi);
}

static void shellStep(ShellState& shellState, double deltaTime) {
}

void shellShutdown() {
}
