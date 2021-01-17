#pragma once

#include <cassert>
#include "primitives.hpp"
#include <glm/glm.hpp>

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
Result<RegionMemoryBuffer> create_region_memory_buffer(u64 size);

Result<StackMemoryBuffer> create_stack_memory_buffer(RegionMemoryBuffer* root, u64 size);

Result<ArenaMemoryBuffer> create_arena_memory_buffer(RegionMemoryBuffer* root, u64 size, u64 chunkSize);

void region_memory_buffer_add_region(RegionMemoryBuffer* where, RegionMemoryBuffer* buffer, u64 size);

Result<u8*> region_memory_buffer_alloc(RegionMemoryBuffer* buffer, u64 size);

void region_memory_buffer_free(RegionMemoryBuffer* buffer);

Result<Arena*> arena_memory_buffer_alloc(ArenaMemoryBuffer* buffer);

void arena_memory_buffer_free(ArenaMemoryBuffer* buffer, Arena* arena);

Result<bool> init_memory();

template<typename T>
Result<u8*> region_memory_push_struct(RegionMemoryBuffer* buffer, T data) {
    auto dataResult = region_memory_buffer_alloc(buffer, sizeof(T));

    if (result_is_success(dataResult)) {
        auto base = result_get_payload(dataResult);
        memcpy(base, &data, sizeof(T));
    }

    return dataResult;
}

template<typename T>
Result<u8*> region_memory_push_chunk(RegionMemoryBuffer* buffer, T* data, size_t len) {
    auto dataResult = region_memory_buffer_alloc(buffer, sizeof(T) * len);

    if (result_is_success(dataResult)) {
        auto base = result_get_payload(dataResult);
        memcpy(base, data, sizeof(T) * len);
    }

    return dataResult;
}
