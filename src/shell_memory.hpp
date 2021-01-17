#pragma once

#include "memory.hpp"

struct ShellMemory {
    RegionMemoryBuffer root_buffer;
    RegionMemoryBuffer assets_buffer;
    RegionMemoryBuffer frame_buffer;
};
