#pragma once

enum class ErrorCode {
    PlatformInit,
    MemoryInit,
    Allocation,
    CreateWindow,
    LoadAsset,
    GApiCreateContext,
    GApiInit,
    GApiCreateShader,
    GApiShaderStatus,
    GApiShaderProgramStatus,
    GApiShaderUniformLocation,
};
