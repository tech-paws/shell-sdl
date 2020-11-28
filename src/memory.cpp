#include "memory.hpp"
#include "platform.hpp"

Result<RegionMemoryBuffer> createRegionMemoryBuffer(u64 size) {
    u8* base = platformAlloc(size);

    if (base) {
        auto buffer = RegionMemoryBuffer {
            .base = base,
            .size = size,
            .offset = 0,
        };
        return resultCreateSuccess<RegionMemoryBuffer>(buffer);
    }
    else {
        return resultCreateGeneralError<RegionMemoryBuffer>(
            ErrorCode::Allocation,
            "platformAlloc has failed"
        );
    }
}

Result<StackMemoryBuffer> createStackMemoryBuffer(RegionMemoryBuffer* root, u64 size) {
    StackMemoryBuffer buffer;

    if (root->offset + size > root->size) {
        return resultCreateGeneralError<StackMemoryBuffer>(
            ErrorCode::Allocation,
            "Out of memory"
        );
    }

    root->offset += size;
    return resultCreateSuccess(buffer);
}

Result<ArenaMemoryBuffer> createArenaMemoryBuffer(RegionMemoryBuffer* where, u64 count, u64 chunkSize) {
    const u64 fullChunkSize = chunkSize + sizeof(Arena);
    Result<u8*> baseResult = regionMemoryBufferAlloc(where, count * fullChunkSize);

    if (resultHasError(baseResult)) {
        return switchError<ArenaMemoryBuffer>(baseResult);
    }
    else {
        u8* base = resultUnwrap(baseResult);
        ArenaMemoryBuffer buffer;
        buffer.head = (Arena*) base;
        Arena* current = buffer.head;

        for (size_t i = 1; i < count; ++i) {
            current->next = (Arena*) (base + (i * fullChunkSize));
            current = current->next;
        }

        current->next = nullptr;
        return resultCreateSuccess(buffer);
    }
}

void regionMemoryBufferAddRegion(RegionMemoryBuffer* where, RegionMemoryBuffer* buffer, u64 size) {
    assert(where->offset + size <= where->size);

    buffer->base = where->base + where->offset;
    buffer->size = size;
    buffer->offset = 0;

    where->offset += size;
}

Result<u8*> regionMemoryBufferAlloc(RegionMemoryBuffer* buffer, u64 size) {
    assert(buffer != nullptr);

    if (buffer->offset + size > buffer->size) {
        return resultCreateGeneralError<u8*>(
            ErrorCode::Allocation,
            "Out of memory"
        );
    }

    // TODO: Alignment
    u8* result = buffer->base + buffer->offset;
    buffer->offset += size;

    return resultCreateSuccess(result);
}

void regionMemoryBufferFree(RegionMemoryBuffer* buffer) {
    buffer->offset = 0;
}

Result<Arena*> arenaMemoryBufferAlloc(ArenaMemoryBuffer* buffer) {
    assert(buffer != nullptr);

    if (buffer->head == nullptr) {
        return resultCreateGeneralError<Arena*>(
            ErrorCode::Allocation,
            "Out of memory"
        );
    }
    else {
        Arena* arena = buffer->head->next;
        return resultCreateSuccess(arena);
    }
}

void arenaMemoryBufferFree(ArenaMemoryBuffer* buffer, Arena* arena) {
    assert(buffer != nullptr);
    assert(arena != nullptr);

    arena->next = buffer->head;
    buffer->head = arena;
}
