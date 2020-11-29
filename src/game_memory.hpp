#pragma once

#include "memory.hpp"

struct GameMemory {
    RegionMemoryBuffer gapiCommandsBuffer;
    RegionMemoryBuffer gapiCommandsDataBuffer;
    RegionMemoryBuffer rootBuffer;
    RegionMemoryBuffer assetsBuffer;
    RegionMemoryBuffer frameBuffer;
};
