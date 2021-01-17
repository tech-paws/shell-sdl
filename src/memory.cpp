#include "memory.hpp"
#include "platform.hpp"

Result<RegionMemoryBuffer> create_region_memory_buffer(u64 size) {
    u8* base = platform_alloc(size);

    if (base) {
        auto buffer = RegionMemoryBuffer {
            .base = base,
            .size = size,
            .offset = 0,
        };
        return result_create_success<RegionMemoryBuffer>(buffer);
    }
    else {
        return result_create_general_error<RegionMemoryBuffer>(
            ErrorCode::Allocation,
            "platform_alloc has failed"
        );
    }
}

Result<StackMemoryBuffer> create_stack_memory_buffer(RegionMemoryBuffer* root, u64 size) {
    StackMemoryBuffer buffer;

    if (root->offset + size > root->size) {
        return result_create_general_error<StackMemoryBuffer>(
            ErrorCode::Allocation,
            "Out of memory"
        );
    }

    root->offset += size;
    return result_create_success(buffer);
}

Result<ArenaMemoryBuffer> create_arena_memory_buffer(RegionMemoryBuffer* where, u64 count, u64 chunk_size) {
    const u64 full_chunk_size = chunk_size + sizeof(Arena);
    Result<u8*> base_result = region_memory_buffer_alloc(where, count * full_chunk_size);

    if (result_has_error(base_result)) {
        return switch_error<ArenaMemoryBuffer>(base_result);
    }
    else {
        u8* base = result_unwrap(base_result);
        ArenaMemoryBuffer buffer;
        buffer.head = (Arena*) base;
        Arena* current = buffer.head;

        for (size_t i = 1; i < count; ++i) {
            current->next = (Arena*) (base + (i * full_chunk_size));
            current = current->next;
        }

        current->next = nullptr;
        return result_create_success(buffer);
    }
}

void region_memory_buffer_add_region(RegionMemoryBuffer* where, RegionMemoryBuffer* buffer, u64 size) {
    assert(where->offset + size <= where->size);

    buffer->base = where->base + where->offset;
    buffer->size = size;
    buffer->offset = 0;

    where->offset += size;
}

Result<u8*> region_memory_buffer_alloc(RegionMemoryBuffer* buffer, u64 size) {
    assert(buffer != nullptr);

    if (buffer->offset + size > buffer->size) {
        return result_create_general_error<u8*>(
            ErrorCode::Allocation,
            "Out of memory"
        );
    }

    // TODO: Alignment
    u8* result = buffer->base + buffer->offset;
    buffer->offset += size;

    return result_create_success(result);
}

void region_memory_buffer_free(RegionMemoryBuffer* buffer) {
    buffer->offset = 0;
}

Result<Arena*> arena_memory_buffer_alloc(ArenaMemoryBuffer* buffer) {
    assert(buffer != nullptr);

    if (buffer->head == nullptr) {
        return result_create_general_error<Arena*>(
            ErrorCode::Allocation,
            "Out of memory"
        );
    }
    else {
        Arena* arena = buffer->head->next;
        return result_create_success(arena);
    }
}

void arena_memory_buffer_free(ArenaMemoryBuffer* buffer, Arena* arena) {
    assert(buffer != nullptr);
    assert(arena != nullptr);

    arena->next = buffer->head;
    buffer->head = arena;
}
