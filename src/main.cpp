#include <stdio.h>

#include "platform.hpp"
#include "game.hpp"

int main() {
    auto platformInitResult = platformInit();

    if (resultIsSuccess(platformInitResult)) {
        auto platform = resultGetPayload(platformInitResult);
        auto createWindowResult = platformCreateWindow(platform);

        if (resultIsSuccess(createWindowResult)) {
            auto window = resultGetPayload(createWindowResult);
            bool running = true;
            auto gameStateResult = gameInit();

            if (resultIsSuccess(gameStateResult)) {
                auto gameState = resultGetPayload(gameStateResult);

                while (running) {
                    running = platformEventLoop(platform, window);
                    gameMainLoop(gameState, platform, window);
                }
            }
            else {
                // TODO(sysint64): log
                puts(createWindowResult.error.message);
            }

            platformDestroyWindow(window);
        }
        else {
            // TODO(sysint64): log
            puts(createWindowResult.error.message);
        }
    }
    else {
        // TODO(sysint64): log
        puts(platformInitResult.error.message);
    }

    puts("Successfully finished");
    return 0;
}
