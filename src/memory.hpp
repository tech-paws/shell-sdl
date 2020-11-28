#pragma once

#include <cassert>
#include "primitives.hpp"

struct RegionMemoryBuffer {
    u8* base;
    u64 size;
    size_t offset;
};

struct StackMemoryBuffer {
    u8* base;
    u64 size;
    size_t offset;
};

struct Arena {
    u8* data;
    Arena* next;
};

struct ArenaMemoryBuffer {
    Arena* head;
};

// API
Result<RegionMemoryBuffer> createRegionMemoryBuffer(u64 size);

Result<StackMemoryBuffer> createStackMemoryBuffer(RegionMemoryBuffer* root, u64 size);

Result<ArenaMemoryBuffer> createArenaMemoryBuffer(RegionMemoryBuffer* root, u64 size, u64 chunkSize);

void regionMemoryBufferAddRegion(RegionMemoryBuffer* where, RegionMemoryBuffer* buffer, u64 size);

Result<u8*> regionMemoryBufferAlloc(RegionMemoryBuffer* buffer, u64 size);

void regionMemoryBufferFree(RegionMemoryBuffer* buffer);

Result<Arena*> arenaMemoryBufferAlloc(ArenaMemoryBuffer* buffer);

void arenaMemoryBufferFree(ArenaMemoryBuffer* buffer, Arena* arena);

Result<bool> initMemory();

template<typename T>
Result<u8*> regionMemoryPushStruct(RegionMemoryBuffer* buffer, T data) {
    auto dataResult = regionMemoryBufferAlloc(buffer, sizeof(T));

    if (resultIsSuccess(dataResult)) {
        auto data = resultGetPayload(dataResult);
        memcpy(data, &data, sizeof(T));
    }

    return dataResult;
}

template<typename T>
Result<u8*> regionMemoryPushChunk(RegionMemoryBuffer* buffer, T* data, size_t len) {
    auto dataResult = regionMemoryBufferAlloc(buffer, sizeof(T) * len);

    if (resultIsSuccess(dataResult)) {
        auto data = resultGetPayload(dataResult);
        memcpy(data, data, sizeof(T) * len);
    }

    return dataResult;
}
