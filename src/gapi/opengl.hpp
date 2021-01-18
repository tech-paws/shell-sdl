#pragma once

#include <GL/glew.h>
#include "memory.hpp"
#include "shell_config.hpp"

enum class ShaderType {
    vertex,
    fragment,
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
    ShellConfig config;
    RegionMemoryBuffer memory_shaders;

    Shader shaders[3];
    ShaderProgram shader_programs[2];
    u32 shader_uniform_locations[4];
    GLuint buffers[8];

    ShaderProgram shader_program_texture;
    ShaderProgram shader_program_color;

    GLuint quad_indices_buffer;
    GLuint quad_vertices_buffer;
    GLuint quad_tex_coords_buffer;
    GLuint quad_vao;

    GLuint centered_quad_indices_buffer;
    GLuint centered_quad_vertices_buffer;
    GLuint centered_quad_tex_coords_buffer;
    GLuint centered_quad_vao;

    size_t mvp_uniform_location_id;
};
