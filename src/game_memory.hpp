#pragma once

#include "memory.hpp"

struct GameMemory {
    RegionMemoryBuffer gapi_commands_buffer;
    RegionMemoryBuffer gapi_commands_data_buffer;
    RegionMemoryBuffer root_buffer;
    RegionMemoryBuffer assets_buffer;
    RegionMemoryBuffer frame_buffer;
};
