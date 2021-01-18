#include <stdio.h>

#include "platform.hpp"
#include "shell.hpp"
#include "vm.hpp"

extern "C" void sdl2shell_run(ShellConfig config) {
    auto platform_init_result = platform_init();

    if (result_is_success(platform_init_result)) {
        auto platform = result_get_payload(platform_init_result);
        auto create_window_result = platform_create_window(config, platform);

        if (result_is_success(create_window_result)) {
            auto window = result_get_payload(create_window_result);
            bool running = true;
            auto shell_state_result = shell_init(config);

            if (result_is_success(shell_state_result)) {
                auto shell_state = result_get_payload(shell_state_result);

                while (running) {
                    running = platform_event_loop(platform, window);
                    shell_main_loop(shell_state, platform, window);
                }
            }
            else {
                // TODO(sysint64): log
                puts(create_window_result.error.message);
            }

            platform_destroy_window(window);
        }
        else {
            // TODO(sysint64): log
            puts(create_window_result.error.message);
        }
    }
    else {
        // TODO(sysint64): log
        puts(platform_init_result.error.message);
    }

    puts("Successfully finished");
}
