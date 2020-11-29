#include "primitives.hpp"
#include "gapi/opengl.hpp"
#include "platform.hpp"
#include "assets.hpp"
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

static Result<GLint> checkShaderStatus(const Shader shader, const GLenum pname) {
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

        return resultCreateGeneralError<GLint>(
            ErrorCode::GApiShaderStatus,
            "%s. shader name: '%s', status: %d, message: '%s'",
            reason, shader.name, status, message
        );
    }

    return resultCreateSuccess(status);
}

static Result<Shader> gapiCreateShader(const char* name, const ShaderType shaderType, AssetData data) {
    Shader shader;
    shader.name = name;
    GLenum glShaderType;

    switch (shaderType) {
        case ShaderType::fragment:
            glShaderType = GL_FRAGMENT_SHADER;
            break;

        case ShaderType::vertex:
            glShaderType = GL_VERTEX_SHADER;
            break;

        default:
            return resultCreateGeneralError<Shader>(
                ErrorCode::GApiCreateShader,
                "Unknown shader type %d", shaderType
            );
    }

    shader.id = glCreateShader(glShaderType);

    char* source = (char*) data.data;
    glShaderSource(shader.id, 1, &source, nullptr);
    glCompileShader(shader.id);
    const auto statusReault = checkShaderStatus(shader, GL_COMPILE_STATUS);

    if (resultHasError(statusReault)) {
        return switchError<Shader>(statusReault);
    }

    return resultCreateSuccess(shader);
}

static Result<GLint> checkProgramStatus(const ShaderProgram program, const GLenum pname) {
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

        return resultCreateGeneralError<GLint>(
            ErrorCode::GApiShaderProgramStatus,
            "%s. program name: '%s', status: %d, message: '%s'",
            reason, program.name, status, message
        );
    }

    return resultCreateSuccess(status);
}

static Result<ShaderProgram> gapiCreateShaderProgram(const char* name, Shader* shaders, u64 count) {
    ShaderProgram program;

    program.id = glCreateProgram();
    program.name = name;

    for (size_t i = 0; i < count; i += 1) {
        glAttachShader(program.id, shaders[i].id);
        puts(shaders[i].name);
    }

    glLinkProgram(program.id);
    const auto statusReault = checkProgramStatus(program, GL_LINK_STATUS);

    if (resultHasError(statusReault)) {
        return switchError<ShaderProgram>(statusReault);
    }

    return resultCreateSuccess(program);
}

static Result<u32> gapiGetShaderUniformLocation(const ShaderProgram program, const char* location) {
    const GLint loc = glGetUniformLocation(program.id, location);

    if (loc == -1) {
        return resultCreateGeneralError<u32>(
            ErrorCode::GApiShaderUniformLocation,
            "Failed to get uniform location. name '%s', location: %s",
            program.name, location
        );
    }
    else {
        return resultCreateSuccess((u32) loc);
    }
}

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

static void initCenteredQuad(GApi& gapi) {
    createBuffer(&gapi.quadIndicesBuffer, &quadIndices[0], sizeof(u32) * quadIndicesCount, false);
    createBuffer(&gapi.quadVerticesBuffer, &quadVertices[0], sizeof(f32) * 2 * quadVerticesCount, false);
    createBuffer(&gapi.quadTexCoordsBuffer, &quadTexCoords[0], sizeof(f32) * 2 * quadTexCoordsCount, false);
    glGenVertexArrays(1, &gapi.quadVao);

    glBindVertexArray(gapi.quadVao);
    gapiCreateVector2fVAO(gapi.quadVerticesBuffer, 0);
    gapiCreateVector2fVAO(gapi.quadTexCoordsBuffer, 1);
}

static void initQuad(GApi& gapi) {
    createBuffer(&gapi.centeredQuadIndicesBuffer, &quadIndices[0], sizeof(u32) * quadIndicesCount, false);
    createBuffer(&gapi.centeredQuadVerticesBuffer, &centeredQuadVertices[0], sizeof(f32) * 2 * quadVerticesCount, false);
    createBuffer(&gapi.centeredQuadTexCoordsBuffer, &quadTexCoords[0], sizeof(f32) * 2 * quadTexCoordsCount, false);
    glGenVertexArrays(1, &gapi.centeredQuadVao);

    glBindVertexArray(gapi.centeredQuadVao);
    gapiCreateVector2fVAO(gapi.centeredQuadVerticesBuffer, 0);
    gapiCreateVector2fVAO(gapi.centeredQuadTexCoordsBuffer, 1);
}

static Result<Shader> gapiLoadShader(GApi& gapi, const char* name, const char* fileName, ShaderType type) {
    const Result<AssetData> shaderAssetResult = assetLoadData(
        &gapi.memoryShaders,
        AssetType::shader,
        fileName
    );

    if (resultHasError(shaderAssetResult)) {
        return switchError<Shader>(shaderAssetResult);
    }

    const auto shaderAsset = resultGetPayload(shaderAssetResult);
    const auto shaderResult = gapiCreateShader(name, type, shaderAsset);

    if (resultHasError(shaderResult)) {
        return switchError<Shader>(shaderResult);
    }

    return shaderResult;
}

static Result<bool> initFragmentColorShader(GApi& gapi) {
    const auto shaderResult = gapiLoadShader(
        gapi,
        "Fragment Color",
        "fragment_color.glsl",
        ShaderType::fragment
    );

    if (resultHasError(shaderResult)) {
        return switchError<bool>(shaderResult);
    }

    gapi.shaderFragmentColor = resultGetPayload(shaderResult);
    return resultCreateSuccess(true);
}

static Result<bool> initFragmentTextureShader(GApi& gapi) {
    const auto shaderResult = gapiLoadShader(
        gapi,
        "Fragment Texture",
        "fragment_texture.glsl",
        ShaderType::fragment
    );

    if (resultHasError(shaderResult)) {
        return switchError<bool>(shaderResult);
    }

    gapi.shaderFragmentTexture = resultGetPayload(shaderResult);
    return resultCreateSuccess(true);
}

static Result<bool> initVertexTransformShader(GApi& gapi) {
    const auto shaderResult = gapiLoadShader(
        gapi,
        "Vertex Transform",
        "vertex_transform.glsl",
        ShaderType::vertex
    );

    if (resultHasError(shaderResult)) {
        return switchError<bool>(shaderResult);
    }

    gapi.shaderVertexTransform = resultGetPayload(shaderResult);
    return resultCreateSuccess(true);
}

static Result<bool> initColorShaderProgram(GApi& gapi) {
    Shader shaders[2];
    shaders[0] = gapi.shaderVertexTransform;
    shaders[1] = gapi.shaderFragmentColor;
    const auto programResult = gapiCreateShaderProgram("Color Shader Program", &shaders[0], 2);

    if (resultHasError(programResult)) {
        return switchError<bool>(programResult);
    }

    auto program = resultGetPayload(programResult);

    Result<u32> locationResult;
    locationResult = gapiGetShaderUniformLocation(program, "MVP");

    if (resultHasError(locationResult)) {
        return switchError<bool>(locationResult);
    }

    gapi.locationColorShaderMVP = resultGetPayload(locationResult);

    locationResult = gapiGetShaderUniformLocation(program, "color");

    if (resultHasError(locationResult)) {
        return switchError<bool>(locationResult);
    }

    gapi.locationColorShaderColor = resultGetPayload(locationResult);

    gapi.shaderProgramColor = program;
    return resultCreateSuccess(true);
}

static Result<bool> initTextureShaderProgram(GApi& gapi) {
    Shader shaders[2];
    shaders[0] = gapi.shaderVertexTransform;
    shaders[1] = gapi.shaderFragmentTexture;
    const auto programResult = gapiCreateShaderProgram("Texture Shader Program", &shaders[0], 2);

    if (resultHasError(programResult)) {
        return switchError<bool>(programResult);
    }

    auto program = resultGetPayload(programResult);

    Result<u32> locationResult;
    locationResult = gapiGetShaderUniformLocation(program, "MVP");

    if (resultHasError(locationResult)) {
        return switchError<bool>(locationResult);
    }

    gapi.locationTextureShaderMVP = resultGetPayload(locationResult);

    locationResult = gapiGetShaderUniformLocation(program, "utexture");

    if (resultHasError(locationResult)) {
        return switchError<bool>(locationResult);
    }

    gapi.locationTextureShaderTexture = resultGetPayload(locationResult);

    gapi.shaderProgramTexture = program;
    return resultCreateSuccess(true);
}

Result<GApi> gapiInit() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto bufferResult = createRegionMemoryBuffer(megabytes(10));

    if (resultIsSuccess(bufferResult)) {
        GApi gapi;
        gapi.memoryShaders = resultGetPayload(bufferResult);

        Result<bool> initComponentResult;

        // Geometry
        initQuad(gapi);
        initCenteredQuad(gapi);

        // Shader
        initComponentResult = initFragmentColorShader(gapi);

        if (resultHasError(initComponentResult)) {
            return switchError<GApi>(initComponentResult);
        }

        initComponentResult = initFragmentTextureShader(gapi);

        if (resultHasError(initComponentResult)) {
            return switchError<GApi>(initComponentResult);
        }

        initComponentResult = initVertexTransformShader(gapi);

        if (resultHasError(initComponentResult)) {
            return switchError<GApi>(initComponentResult);
        }

        initComponentResult = initColorShaderProgram(gapi);

        if (resultHasError(initComponentResult)) {
            return switchError<GApi>(initComponentResult);
        }

        initComponentResult = initTextureShaderProgram(gapi);

        if (resultHasError(initComponentResult)) {
            return switchError<GApi>(initComponentResult);
        }

        return resultCreateSuccess(gapi);
    } else {
        return switchError<GApi>(bufferResult);
    }
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
