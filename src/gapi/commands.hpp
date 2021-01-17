#pragma once

#include <glm/glm.hpp>
#include "game_memory.hpp"

// General
static const u64 GAPI_COMMAND_EXECUTE_MACRO = 0x0001;
static const u64 GAPI_COMMAND_BEGIN_MACRO = 0x0002;
static const u64 GAPI_COMMAND_END_MACRO = 0x0003;

// Pipelines
static const u64 GAPI_COMMAND_SET_COLOR_PIPELINE = 0x0101;
static const u64 GAPI_COMMAND_SET_TEXTURE_PIPELINE = 0x0102;

// Draw
static const u64 GAPI_COMMAND_DRAW_LINES = 0x0201;
static const u64 GAPI_COMMAND_DRAW_PATH = 0x0202;
static const u64 GAPI_COMMAND_DRAW_QUADS = 0x0203;
static const u64 GAPI_COMMAND_DRAW_CENTERED_QUADS = 0x0204;
static const u64 GAPI_COMMAND_DRAW_TEXTS = 0x0205;

struct GAPICommandPayload {
    u64 size;
    u8* base;
};

struct GAPICommand {
    u64 id;
    GAPICommandPayload payload;
};

static const GAPICommandPayload gapi_empty_command_payload = {0, nullptr};

inline void gapi_push_command(GameMemory& memory, u64 id, GAPICommandPayload payload) {
    GAPICommand command = { id, payload };
    auto result = region_memory_push_struct(&memory.gapi_commands_buffer, command);
    result_unwrap(result);
}

template<typename T>
GAPICommandPayload gapi_create_command_payload(GameMemory& memory, T* values, size_t len) {
    auto dataResult = region_memory_push_chunk(&memory.gapi_commands_data_buffer, values, len);
    auto data = result_unwrap(dataResult);
    return { len * sizeof(T), data };
}

// General

inline void gapi_execute_macro(GameMemory& memory, u64 id) {
    auto payload = gapi_create_command_payload(memory, &id, 1);
    gapi_push_command(memory, GAPI_COMMAND_EXECUTE_MACRO, payload);
}

inline void gapi_begin_macro(GameMemory& memory, u64 id) {
    auto payload = gapi_create_command_payload(memory, &id, 1);
    gapi_push_command(memory, GAPI_COMMAND_BEGIN_MACRO, payload);
}

inline void gapi_end_macro(GameMemory& memory, u64 id) {
    auto payload = gapi_create_command_payload(memory, &id, 1);
    gapi_push_command(memory, GAPI_COMMAND_END_MACRO, payload);
}

// Pipelines

inline void gapi_set_color_pipeline(GameMemory& memory, glm::vec4 color) {
    auto payload = gapi_create_command_payload(memory, &color, 1);
    gapi_push_command(memory, GAPI_COMMAND_SET_COLOR_PIPELINE, payload);
}

inline void gapi_set_texture_pipeline(GameMemory& memory, u64 textureId) {
    auto payload = gapi_create_command_payload(memory, &textureId, 1);
    gapi_push_command(memory, GAPI_COMMAND_SET_TEXTURE_PIPELINE, payload);
}

// Draw

inline void draw_lines(GameMemory& memory, glm::vec3* points, size_t count) {
    auto payload = gapi_create_command_payload(memory, points, count);
    gapi_push_command(memory, GAPI_COMMAND_DRAW_LINES, payload);
}

inline void draw_path(GameMemory& memory, glm::vec3* points, size_t count) {
    auto payload = gapi_create_command_payload(memory, points, count);
    gapi_push_command(memory, GAPI_COMMAND_DRAW_PATH, payload);
}

inline void draw_quads(GameMemory& memory, glm::mat4* quadsMvpMatrices, size_t count) {
    auto payload = gapi_create_command_payload(memory, quadsMvpMatrices, count);
    gapi_push_command(memory, GAPI_COMMAND_DRAW_QUADS, payload);
}

inline void draw_centered_quads(GameMemory& memory, glm::mat4* quadsMvpMatrices, size_t count) {
    auto payload = gapi_create_command_payload(memory, quadsMvpMatrices, count);
    gapi_push_command(memory, GAPI_COMMAND_DRAW_CENTERED_QUADS, payload);
}

inline void draw_texts(GameMemory& memory, String* texts, size_t count) {
    auto payload = gapi_create_command_payload(memory, texts, count);
    gapi_push_command(memory, GAPI_COMMAND_DRAW_TEXTS, payload);
}
