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

struct GApi {
    RegionMemoryBuffer memoryShaders;

    Shader shaderFragmentColor;
    Shader shaderFragmentTexture;
    Shader shaderVertexTransform;
    ShaderProgram shaderProgramTexture;
    ShaderProgram shaderProgramColor;

    u32 locationTextureShaderMVP;
    u32 locationTextureShaderTexture;
    u32 locationColorShaderMVP;
    u32 locationColorShaderColor;

    GLuint quadIndicesBuffer;
    GLuint quadVerticesBuffer;
    GLuint quadTexCoordsBuffer;
    GLuint quadVao;

    GLuint centeredQuadIndicesBuffer;
    GLuint centeredQuadVerticesBuffer;
    GLuint centeredQuadTexCoordsBuffer;
    GLuint centeredQuadVao;
};
