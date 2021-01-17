#include "primitives.hpp"
#include "gapi/opengl.hpp"
#include "gapi/commands.hpp"
#include "platform.hpp"
#include "assets.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

const size_t quad_vertices_count = 4;
const glm::vec2 centered_quad_vertices[quad_vertices_count] = {
    glm::vec2(-0.5f, -0.5f),
    glm::vec2( 0.5f, -0.5f),
    glm::vec2( 0.5f,  0.5f),
    glm::vec2(-0.5f,  0.5f),
};

const glm::vec2 quad_vertices[quad_vertices_count] = {
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 1.0f),
};

const size_t quad_indices_count = 4;
const u32 quad_indices[quad_indices_count] = { 0, 3, 1, 2 };

const size_t quad_tex_coords_count = 4;
const glm::vec2 quad_tex_coords[quad_tex_coords_count] = {
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 0.0f),
};

static Result<GLint> check_shader_status(const Shader shader, const GLenum pname) {
    GLint status, length;
    GLchar message[1024] { 0 };

    glGetShaderiv(shader.id, pname, &status);

    if (status != GL_TRUE) {
        glGetShaderInfoLog(shader.id, 1024, &length, &message[0]);
        const char* reason;

        switch (pname) {
            case GL_COMPILE_STATUS:
                reason = "Failed when compiling shader";
                break;

            default:
                reason = "Failed";
                break;
        }

        return result_create_general_error<GLint>(
            ErrorCode::GApiShaderStatus,
            "%s. shader name: '%s', status: %d, message: '%s'",
            reason, shader.name, status, message
        );
    }

    return result_create_success(status);
}

static Result<Shader> gapi_create_shader(const char* name, const ShaderType shader_type, AssetData data) {
    Shader shader;
    shader.name = name;
    GLenum gl_shader_type;

    switch (shader_type) {
        case ShaderType::fragment:
            gl_shader_type = GL_FRAGMENT_SHADER;
            break;

        case ShaderType::vertex:
            gl_shader_type = GL_VERTEX_SHADER;
            break;

        default:
            return result_create_general_error<Shader>(
                ErrorCode::GApiCreateShader,
                "Unknown shader type %d", shader_type
            );
    }

    shader.id = glCreateShader(gl_shader_type);

    char* source = (char*) data.data;
    glShaderSource(shader.id, 1, &source, nullptr);
    glCompileShader(shader.id);
    const auto status_reault = check_shader_status(shader, GL_COMPILE_STATUS);

    if (result_has_error(status_reault)) {
        return switch_error<Shader>(status_reault);
    }

    return result_create_success(shader);
}

static Result<GLint> check_program_status(const ShaderProgram program, const GLenum pname) {
    GLint status, length;
    GLchar message[1024] { 0 };

    glGetProgramiv(program.id, pname, &status);

    if (status != GL_TRUE) {
        glGetProgramInfoLog(program.id, 1024, &length, &message[0]);
        const char* reason;

        switch (pname) {
            case GL_VALIDATE_STATUS:
                reason = "Failed when validation shader program";
                break;

            case GL_LINK_STATUS:
                reason = "Failed on linking program";
                break;

            default:
                reason = "Failed";
        }

        return result_create_general_error<GLint>(
            ErrorCode::GApiShaderProgramStatus,
            "%s. program name: '%s', status: %d, message: '%s'",
            reason, program.name, status, message
        );
    }

    return result_create_success(status);
}

static Result<ShaderProgram> gapi_create_shader_program(const char* name, Shader* shaders, u64 count) {
    ShaderProgram program;

    program.id = glCreateProgram();
    program.name = name;

    for (size_t i = 0; i < count; i += 1) {
        glAttachShader(program.id, shaders[i].id);
    }

    glLinkProgram(program.id);
    const auto status_reault = check_program_status(program, GL_LINK_STATUS);

    if (result_has_error(status_reault)) {
        return switch_error<ShaderProgram>(status_reault);
    }

    return result_create_success(program);
}

static Result<u32> gapi_get_shader_uniform_location(const ShaderProgram program, const char* location) {
    const GLint loc = glGetUniformLocation(program.id, location);

    if (loc == -1) {
        return result_create_general_error<u32>(
            ErrorCode::GApiShaderUniformLocation,
            "Failed to get uniform location. name '%s', location: %s",
            program.name, location
        );
    }
    else {
        return result_create_success((u32) loc);
    }
}

static void create_buffer(GLuint* buffer, const void* data, size_t size, bool is_dynamic) {
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);

    if (is_dynamic) {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    } else {
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}

static void gapi_create_vector2f_vao(GLuint buffer, u32 location) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

static void init_centered_quad(GApi& gapi) {
    create_buffer(&gapi.quad_indices_buffer, &quad_indices[0], sizeof(u32) * quad_indices_count, false);
    create_buffer(&gapi.quad_vertices_buffer, &quad_vertices[0], sizeof(f32) * 2 * quad_vertices_count, false);
    create_buffer(&gapi.quad_tex_coords_buffer, &quad_tex_coords[0], sizeof(f32) * 2 * quad_tex_coords_count, false);
    glGenVertexArrays(1, &gapi.quad_vao);

    glBindVertexArray(gapi.quad_vao);
    gapi_create_vector2f_vao(gapi.quad_vertices_buffer, 0);
    gapi_create_vector2f_vao(gapi.quad_tex_coords_buffer, 1);
}

static void init_quad(GApi& gapi) {
    create_buffer(&gapi.centered_quad_indices_buffer, &quad_indices[0], sizeof(u32) * quad_indices_count, false);
    create_buffer(&gapi.centered_quad_vertices_buffer, &centered_quad_vertices[0], sizeof(f32) * 2 * quad_vertices_count, false);
    create_buffer(&gapi.centered_quad_tex_coords_buffer, &quad_tex_coords[0], sizeof(f32) * 2 * quad_tex_coords_count, false);
    glGenVertexArrays(1, &gapi.centered_quad_vao);

    glBindVertexArray(gapi.centered_quad_vao);
    gapi_create_vector2f_vao(gapi.centered_quad_vertices_buffer, 0);
    gapi_create_vector2f_vao(gapi.centered_quad_tex_coords_buffer, 1);
}

static Result<bool> gapi_load_shader(GApi& gapi, size_t id, const char* name, const char* file_name, ShaderType type) {
    const Result<AssetData> shader_asset_result = asset_load_data(
        &gapi.memory_shaders,
        AssetType::shader,
        file_name
    );

    if (result_has_error(shader_asset_result)) {
        return switch_error<bool>(shader_asset_result);
    }

    const auto shader_asset = result_get_payload(shader_asset_result);
    const auto shader_result = gapi_create_shader(name, type, shader_asset);

    if (result_has_error(shader_result)) {
        return switch_error<bool>(shader_result);
    }

    gapi.shaders[id] = result_get_payload(shader_result);
    return result_create_success(true);
}

inline static Result<bool> init_fragment_color_shader(GApi& gapi) {
    return gapi_load_shader(
        gapi,
        GAPI_SHADER_FRAGMENT_COLOR_ID,
        "Fragment Color",
        "fragment_color.glsl",
        ShaderType::fragment
    );
}

inline static Result<bool> init_fragment_texture_shader(GApi& gapi) {
    return gapi_load_shader(
        gapi,
        GAPI_SHADER_FRAGMENT_TEXTURE_ID,
        "Fragment Texture",
        "fragment_texture.glsl",
        ShaderType::fragment
    );
}

inline static Result<bool> init_vertex_transform_shader(GApi& gapi) {
    return gapi_load_shader(
        gapi,
        GAPI_SHADER_VERTEX_TRANSFORM_ID,
        "Vertex Transform",
        "vertex_transform.glsl",
        ShaderType::vertex
    );
}

static Result<bool> init_shader_uniform_location(GApi& gapi, size_t id, ShaderProgram& program, const char* location) {
    Result<u32> location_result;
    location_result = gapi_get_shader_uniform_location(program, location);

    if (result_has_error(location_result)) {
        return switch_error<bool>(location_result);
    } else {
        gapi.shader_uniform_locations[id] = result_get_payload(location_result);
        return result_create_success(true);
    }
}

static Result<bool> init_color_shader_program(GApi& gapi) {
    Shader shaders[2];
    shaders[0] = gapi.shaders[GAPI_SHADER_VERTEX_TRANSFORM_ID];
    shaders[1] = gapi.shaders[GAPI_SHADER_FRAGMENT_COLOR_ID];
    const auto program_result = gapi_create_shader_program("Color Shader Program", &shaders[0], 2);

    if (result_has_error(program_result)) {
        return switch_error<bool>(program_result);
    }

    auto program = result_get_payload(program_result);

    Result<bool> location_result;
    location_result = init_shader_uniform_location(gapi, GAPI_SHADER_LOCATION_COLOR_SHADER_MVP_ID, program, "MVP");

    if (result_has_error(location_result)) {
        return location_result;
    }

    location_result = init_shader_uniform_location(gapi, GAPI_SHADER_LOCATION_COLOR_SHADER_COLOR_ID, program, "color");

    if (result_has_error(location_result)) {
        return location_result;
    }

    gapi.shader_program_color = program;
    return result_create_success(true);
}

static Result<bool> init_texture_shader_program(GApi& gapi) {
    Shader shaders[2];
    shaders[0] = gapi.shaders[GAPI_SHADER_VERTEX_TRANSFORM_ID];
    shaders[1] = gapi.shaders[GAPI_SHADER_FRAGMENT_TEXTURE_ID];
    const auto program_result = gapi_create_shader_program("Texture Shader Program", &shaders[0], 2);

    if (result_has_error(program_result)) {
        return switch_error<bool>(program_result);
    }

    auto program = result_get_payload(program_result);

    Result<bool> location_result;
    location_result = init_shader_uniform_location(gapi, GAPI_SHADER_LOCATION_TEXTURE_SHADER_MVP_ID, program, "MVP");

    if (result_has_error(location_result)) {
        return location_result;
    }

    location_result = init_shader_uniform_location(gapi, GAPI_SHADER_LOCATION_TEXTURE_SHADER_TEXTURE_ID, program, "utexture");

    if (result_has_error(location_result)) {
        return location_result;
    }

    gapi.shader_program_texture = program;
    return result_create_success(true);
}

Result<GApi> gapi_init() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto buffer_result = create_region_memory_buffer(megabytes(10));

    if (result_is_success(buffer_result)) {
        GApi gapi;
        gapi.memory_shaders = result_get_payload(buffer_result);

        Result<bool> init_component_result;

        // Geometry
        init_quad(gapi);
        init_centered_quad(gapi);

        // Shaders
        init_component_result = init_fragment_color_shader(gapi);

        if (result_has_error(init_component_result)) {
            return switch_error<GApi>(init_component_result);
        }

        init_component_result = init_fragment_texture_shader(gapi);

        if (result_has_error(init_component_result)) {
            return switch_error<GApi>(init_component_result);
        }

        init_component_result = init_vertex_transform_shader(gapi);

        if (result_has_error(init_component_result)) {
            return switch_error<GApi>(init_component_result);
        }

        // Programs
        init_component_result = init_color_shader_program(gapi);

        if (result_has_error(init_component_result)) {
            return switch_error<GApi>(init_component_result);
        }

        init_component_result = init_texture_shader_program(gapi);

        if (result_has_error(init_component_result)) {
            return switch_error<GApi>(init_component_result);
        }

        return result_create_success(gapi);
    } else {
        return switch_error<GApi>(buffer_result);
    }
}

void gapi_clear(float r, float g, float b) {
    glClearColor(r, g, b, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

Texture2D gapi_create_texture_2d(const AssetData data, const Texture2DParameters params) {
    Texture2D texture;
    TextureHeader texture_header = *((TextureHeader*) data.data);
    u8* texture_data = data.data + sizeof(TextureHeader);

    texture.width = texture_header.width;
    texture.height = texture_header.height;

    GLenum format;

    switch (texture_header.format) {
        case TextureFormat::rgb:
            format = GL_RGB;
            break;

        case TextureFormat::rgba:
            format = GL_RGBA;
            break;
    }

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    // TODO: Not sure if I should use it
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        /* target */ GL_TEXTURE_2D,
        /* level */ 0,
        /* internalformat */ format,
        /* width */ texture.width,
        /* height */ texture.height,
        /* border */ 0,
        /* format */ format,
        /* type */ GL_UNSIGNED_BYTE,
        /* data */ texture_data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return texture;
}

void gapi_delete_texture_2d(Texture2D texture) {
    glDeleteTextures(1, &texture.id);
}

static void gapi_set_color_pipeline(GApi& gapi, GAPICommandPayload payload) {

#ifdef VALIDATE
    glValidateProgram(gapi.shader_program_color.id);
    const auto status_reault = check_program_status(gapi.shader_program_color, GL_VALIDATE_STATUS);
    result_unwrap(status_reault);
#endif

    glUseProgram(gapi.shader_program_color.id);

    const auto color = (glm::vec4*) payload.base;
    const auto loc = gapi.shader_uniform_locations[GAPI_SHADER_LOCATION_COLOR_SHADER_COLOR_ID];
    gapi.mvp_uniform_location_id = GAPI_SHADER_LOCATION_COLOR_SHADER_MVP_ID;

    glUniform4fv(loc, 1, glm::value_ptr(*color));
}

static void gapi_set_texture_pipeline(GApi& gapi, GAPICommandPayload payload) {

#ifdef VALIDATE
    glValidateProgram(gapi.shader_program_color.id);
    const auto status_reault = check_program_status(gapi.shader_program_color, GL_VALIDATE_STATUS);
    result_unwrap(status_reault);
#endif

    glUseProgram(gapi.shader_program_texture.id);

    const auto textureId = (u64*) payload.base;
    const auto loc = gapi.shader_uniform_locations[GAPI_SHADER_LOCATION_TEXTURE_SHADER_TEXTURE_ID];
    gapi.mvp_uniform_location_id = GAPI_SHADER_LOCATION_TEXTURE_SHADER_MVP_ID;

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, *textureId);
    glUniform1i(loc, 1);
}

static void gapi_draw_quads(GApi& gapi, GAPICommandPayload payload) {
    u64 cursor = 0;

    glBindVertexArray(gapi.quad_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gapi.quad_indices_buffer);

    while (cursor < payload.size) {
        const auto mvp_mat = (glm::mat4*) &payload.base[cursor];
        const auto loc = gapi.shader_uniform_locations[gapi.mvp_uniform_location_id];

        glUniformMatrix4fv(loc, 1, GL_TRUE, glm::value_ptr(*mvp_mat));
        glDrawElements(GL_TRIANGLE_STRIP, quad_indices_count, GL_UNSIGNED_INT, nullptr);

        cursor += sizeof(glm::mat4);
    }
}

static void gapi_draw_centered_quads(GApi& gapi, GAPICommandPayload payload) {
    u64 cursor = 0;

    glBindVertexArray(gapi.centered_quad_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gapi.quad_indices_buffer);

    while (cursor < payload.size) {
        const auto mvp_mat = (glm::mat4*) &payload.base[cursor];
        const auto loc = gapi.shader_uniform_locations[GAPI_SHADER_LOCATION_COLOR_SHADER_MVP_ID];

        glUniformMatrix4fv(loc, 1, GL_TRUE, glm::value_ptr(*mvp_mat));
        glDrawElements(GL_TRIANGLE_STRIP, quad_indices_count, GL_UNSIGNED_INT, nullptr);

        cursor += sizeof(glm::mat4);
    }
}

void gapi_render(GApi& gapi, GameMemory& memory) {
    auto commands_buffer = &memory.gapi_commands_buffer;
    u64 cursor = 0;

    while (cursor < commands_buffer->offset) {
        auto command = (GAPICommand*) &commands_buffer->base[cursor];

        switch (command->id) {
            case GAPI_COMMAND_DRAW_QUADS:
                gapi_draw_quads(gapi, command->payload);
                break;

            case GAPI_COMMAND_DRAW_CENTERED_QUADS:
                gapi_draw_centered_quads(gapi, command->payload);
                break;

            case GAPI_COMMAND_SET_COLOR_PIPELINE:
                gapi_set_color_pipeline(gapi, command->payload);
                break;

            case GAPI_COMMAND_SET_TEXTURE_PIPELINE:
                gapi_set_texture_pipeline(gapi, command->payload);
                break;

            default:
                printf("Unknown command type: 0x%04lX\n", command->id);
                break;
        }

        cursor += sizeof(GAPICommand);
    }

    region_memory_buffer_free(commands_buffer);
    region_memory_buffer_free(&memory.gapi_commands_data_buffer);
}
