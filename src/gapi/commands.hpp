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

static const GAPICommandPayload gapiEmptyCommandPayload = {0, nullptr};

inline void gapiPushCommand(GameMemory& memory, u64 id, GAPICommandPayload payload) {
    GAPICommand command = { id, payload };
    auto result = regionMemoryPushStruct(&memory.gapiCommandsBuffer, command);
    resultUnwrap(result);
}

template<typename T>
GAPICommandPayload gapiCreateCommandPayload(GameMemory& memory, T* values, size_t len) {
    auto dataResult = regionMemoryPushChunk(&memory.gapiCommandsDataBuffer, values, len);
    auto data = resultUnwrap(dataResult);
    return { len * sizeof(T), data };
}

// General

inline void gapiExecuteMacro(GameMemory& memory, u64 id) {
    auto payload = gapiCreateCommandPayload(memory, &id, 1);
    gapiPushCommand(memory, GAPI_COMMAND_EXECUTE_MACRO, payload);
}

inline void gapiBeginMacro(GameMemory& memory, u64 id) {
    auto payload = gapiCreateCommandPayload(memory, &id, 1);
    gapiPushCommand(memory, GAPI_COMMAND_BEGIN_MACRO, payload);
}

inline void gapiEndMacro(GameMemory& memory, u64 id) {
    auto payload = gapiCreateCommandPayload(memory, &id, 1);
    gapiPushCommand(memory, GAPI_COMMAND_END_MACRO, payload);
}

// Pipelines

inline void gapiSetColorPipeline(GameMemory& memory, glm::vec4 color) {
    auto payload = gapiCreateCommandPayload(memory, &color, 1);
    gapiPushCommand(memory, GAPI_COMMAND_SET_COLOR_PIPELINE, payload);
}

inline void gapiSetTexturePipeline(GameMemory& memory, u64 textureId) {
    auto payload = gapiCreateCommandPayload(memory, &textureId, 1);
    gapiPushCommand(memory, GAPI_COMMAND_SET_TEXTURE_PIPELINE, payload);
}

// Draw

inline void drawLines(GameMemory& memory, glm::vec3* points, size_t count) {
    auto payload = gapiCreateCommandPayload(memory, points, count);
    gapiPushCommand(memory, GAPI_COMMAND_DRAW_LINES, payload);
}

inline void drawPath(GameMemory& memory, glm::vec3* points, size_t count) {
    auto payload = gapiCreateCommandPayload(memory, points, count);
    gapiPushCommand(memory, GAPI_COMMAND_DRAW_PATH, payload);
}

inline void drawQuads(GameMemory& memory, glm::mat4* quadsMvpMatrices, size_t count) {
    auto payload = gapiCreateCommandPayload(memory, quadsMvpMatrices, count);
    gapiPushCommand(memory, GAPI_COMMAND_DRAW_QUADS, payload);
}

inline void drawCenteredQuads(GameMemory& memory, glm::mat4* quadsMvpMatrices, size_t count) {
    auto payload = gapiCreateCommandPayload(memory, quadsMvpMatrices, count);
    gapiPushCommand(memory, GAPI_COMMAND_DRAW_CENTERED_QUADS, payload);
}

inline void drawTexts(GameMemory& memory, String* texts, size_t count) {
    auto payload = gapiCreateCommandPayload(memory, texts, count);
    gapiPushCommand(memory, GAPI_COMMAND_DRAW_TEXTS, payload);
}
