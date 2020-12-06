#pragma once

#include "memory.hpp"

struct ShellMemory {
    RegionMemoryBuffer rootBuffer;
    RegionMemoryBuffer assetsBuffer;
    RegionMemoryBuffer frameBuffer;
};
