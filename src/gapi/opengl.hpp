#pragma once

#include <GL/glew.h>
#include "memory.hpp"

enum class ShaderType {
    vertex,    fragment,
};

struct Shader {
    GLuint id;
    const char* name;
};

struct ShaderProgram {
    GLuint id;
    const char* name;
};

struct Texture2D {
    GLuint id = 0;
    u32 width;
    u32 height;
};

static const size_t GAPI_SHADER_FRAGMENT_COLOR_ID = 0;
static const size_t GAPI_SHADER_FRAGMENT_TEXTURE_ID = 1;
static const size_t GAPI_SHADER_VERTEX_TRANSFORM_ID = 2;

static const size_t GAPI_SHADER_LOCATION_TEXTURE_SHADER_MVP_ID = 0;
static const size_t GAPI_SHADER_LOCATION_TEXTURE_SHADER_TEXTURE_ID = 1;
static const size_t GAPI_SHADER_LOCATION_COLOR_SHADER_MVP_ID = 2;
static const size_t GAPI_SHADER_LOCATION_COLOR_SHADER_COLOR_ID = 3;

struct GApi {
    RegionMemoryBuffer memoryShaders;

    Shader shaders[3];
    ShaderProgram shaderPrograms[2];
    u32 shaderUniformLocations[4];
    GLuint buffers[8];

    ShaderProgram shaderProgramTexture;
    ShaderProgram shaderProgramColor;

    GLuint quadIndicesBuffer;
    GLuint quadVerticesBuffer;
    GLuint quadTexCoordsBuffer;
    GLuint quadVao;

    GLuint centeredQuadIndicesBuffer;
    GLuint centeredQuadVerticesBuffer;
    GLuint centeredQuadTexCoordsBuffer;
    GLuint centeredQuadVao;
};
