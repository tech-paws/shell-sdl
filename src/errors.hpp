#pragma once

enum class ErrorCode {
    PlatformInit,
    MemoryInit,
    Allocation,
    CreateWindow,
    LoadAsset,
    RenderText,
    GetTTFFont,
    GApiCreateContext,
    GApiInit,
    GApiCreateShader,
    GApiShaderStatus,
    GApiShaderProgramStatus,
    GApiShaderUniformLocation,
};
