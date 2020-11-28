#include "primitives.hpp"
#include "gapi/opengl.hpp"
#include "platform.hpp"
#include <glm/glm.hpp>

const size_t quadVerticesCount = 4;
const glm::vec2 centeredQuadVertices[quadVerticesCount] = {
    glm::vec2(-0.5f, -0.5f),
    glm::vec2( 0.5f, -0.5f),
    glm::vec2( 0.5f,  0.5f),
    glm::vec2(-0.5f,  0.5f),
};

const glm::vec2 quadVertices[quadVerticesCount] = {
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 1.0f),
};

const size_t quadIndicesCount = 4;
const u32 quadIndices[quadIndicesCount] = { 0, 3, 1, 2 };

const size_t quadTexCoordsCount = 4;
const glm::vec2 quadTexCoords[quadTexCoordsCount] = {
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 0.0f),
};

static void createBuffer(GLuint* buffer, const void* data, size_t size, bool isDynamic) {
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);

    if (isDynamic) {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    } else {
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}

static void gapiCreateVector2fVAO(GLuint buffer, u32 location) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

Result<GApi> gapiInit() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    GApi gapi;

    gapi.quadVao = 0;

    createBuffer(&gapi.quadIndicesBuffer, &quadIndices[0], sizeof(u32) * quadIndicesCount, false);
    createBuffer(&gapi.quadVerticesBuffer, &centeredQuadVertices[0], sizeof(f32) * 2 * quadVerticesCount, false);
    createBuffer(&gapi.quadTexCoordsBuffer, &quadTexCoords[0], sizeof(f32) * 2 * quadTexCoordsCount, false);
    glGenVertexArrays(1, &gapi.quadVao);

    glBindVertexArray(gapi.quadVao);
    gapiCreateVector2fVAO(gapi.quadVerticesBuffer, 0);
    gapiCreateVector2fVAO(gapi.quadTexCoordsBuffer, 1);

    return resultCreateSuccess(gapi);
}

void gapiClear(float r, float g, float b) {
    glClearColor(r, g, b, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void gapiRender(GApi& gapi, GameState& gameState) {
    auto commandsBuffer = &gameState.memory.gapiCommandsBuffer;

    while (u64 cursor = 0 < commandsBuffer->offset) {
    }

    regionMemoryBufferFree(commandsBuffer);
}
