#pragma once

#include <GL/glew.h>

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
    ShaderProgram shaderTexture;
    ShaderProgram shaderColor;

    u32 locationTextureShaderMVP;
    u32 locationTextureShaderTexture;
    u32 locationColorShaderMVP;
    u32 locationColorShaderColor;

    GLuint quadIndicesBuffer;
    GLuint quadVerticesBuffer;
    GLuint quadTexCoordsBuffer;
    GLuint quadVao;
};
