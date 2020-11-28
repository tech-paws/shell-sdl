#pragma once

#include <glm/glm.hpp>

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
static const u64 GAPI_COMMAND_DRAW_TEXTS = 0x0204;

struct GAPICommandPayload {
    u64 size;
    u8* base;
};

static const GAPICommandPayload gapiEmptyCommandPayload = {0, nullptr};

void gapiPushCommand(u64 id, GAPICommandPayload payload);

template<typename T>
GAPICommandPayload gapiCreateCommandPayload(T* values, size_t len) {
    // TODO(sysint64): implement
}

// General

inline void gapiExecuteMacro(u64 id) {
    auto payload = gapiCreateCommandPayload(&id, 1);
    gapiPushCommand(GAPI_COMMAND_EXECUTE_MACRO, payload);
}

inline void gapiBeginMacro(u64 id) {
    auto payload = gapiCreateCommandPayload(&id, 1);
    gapiPushCommand(GAPI_COMMAND_BEGIN_MACRO, gapiEmptyCommandPayload);
}

inline void gapiEndMacro(u64 id) {
    auto payload = gapiCreateCommandPayload(&id, 1);
    gapiPushCommand(GAPI_COMMAND_END_MACRO, gapiEmptyCommandPayload);
}

// Pipelines

inline void gapiSetColorPipeline(glm::vec4 color) {
    auto payload = gapiCreateCommandPayload(&color, 1);
    gapiPushCommand(GAPI_COMMAND_SET_COLOR_PIPELINE, gapiEmptyCommandPayload);
}

inline void gapiSetTexturePipeline(u64 textureId) {
    auto payload = gapiCreateCommandPayload(&textureId, 1);
    gapiPushCommand(GAPI_COMMAND_SET_TEXTURE_PIPELINE, gapiEmptyCommandPayload);
}

// Draw

inline void drawLines(glm::vec3* points, size_t count) {
    auto payload = gapiCreateCommandPayload(points, count);
    gapiPushCommand(GAPI_COMMAND_DRAW_LINES, payload);
}

inline void drawPath(glm::vec3* points, size_t count) {
    auto payload = gapiCreateCommandPayload(points, count);
    gapiPushCommand(GAPI_COMMAND_DRAW_PATH, payload);
}

inline void drawQuads(glm::vec4* quads, size_t count) {
    auto payload = gapiCreateCommandPayload(quads, count);
    gapiPushCommand(GAPI_COMMAND_DRAW_QUADS, payload);
}

inline void drawTexts(String* texts, size_t count) {
    auto payload = gapiCreateCommandPayload(texts, count);
    gapiPushCommand(GAPI_COMMAND_DRAW_TEXTS, payload);
}
