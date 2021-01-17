#pragma once

struct CommandPayload {
    u64 size;
    u8 const* base;
};

struct Command {
    u64 id;
    CommandPayload payload;
};

struct Commands {
    Command const* data;
    size_t size;
};

struct Vec4f {
    float x;
    float y;
    float z;
    float w;
};

struct Mat4f {
    Vec4f cols[4];
};

inline float const* vec4fptr(Vec4f const* vec) {
    return &vec->x;
}

inline float const* mat4fptr(Mat4f const* mat) {
    return &mat->cols[0].x;
}

extern "C" void tech_paws_vm_init();

extern "C" void tech_paws_vm_process_commands();

extern "C" void tech_paws_vm_process_render_commands();

extern "C" Commands tech_paws_vm_consume_commands();

extern "C" Commands tech_paws_vm_consume_gapi_commands();

extern "C" void tech_paws_vm_gapi_flush();

extern "C" void tech_paws_vm_flush();
