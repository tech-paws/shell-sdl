#pragma once

#include "vm.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

struct CameraMatrices {
    Mat4f view_matrix;
    Mat4f projection_matrix;
    Mat4f model_matrix;
    Mat4f mvp_matrix;
};

struct OthroCameraTransforms {
    Vec2f viewport_size;
    Vec2f position;
    float zoom;
};

struct Transforms2D {
    Vec2f position;
    Vec2f scaling;
    float rotation { 0.f };
};

inline glm::vec2 glm_vec2(Vec2f const& vec) {
    return glm::vec2(vec.x, vec.y);
}

inline glm::vec3 glm_vec3(Vec3f const& vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}

inline glm::vec4 glm_vec4(Vec4f const& vec) {
    return glm::vec4(vec.x, vec.y, vec.z, vec.w);
}

inline Vec2f vm_vec2f(float x, float y) {
    return Vec2f { .x = x, .y = y };
}

inline Vec3f vm_vec3f(float x, float y, float z) {
    return Vec3f { .x = x, .y = y, .z = z };
}

inline Vec4f vm_vec4f(float x, float y, float z, float w) {
    return Vec4f { .x = x, .y = y, .z = z, .w = w };
}

inline Vec2f vm_vec2f(glm::vec2 const& glm_vec) {
    return vm_vec2f(glm_vec.x, glm_vec.y);
}

inline Vec3f vm_vec3f(glm::vec3 const& glm_vec) {
    return vm_vec3f(glm_vec.x, glm_vec.y, glm_vec.z);
}

inline Vec4f vm_vec4f(glm::vec4 const& glm_vec) {
    return vm_vec4f(glm_vec.x, glm_vec.y, glm_vec.z, glm_vec.w);
}

inline Mat4f vm_mat4f(Vec4f c1, Vec4f c2, Vec4f c3, Vec4f c4) {
    return Mat4f { .cols = { c1, c2, c3, c4 } };
}

inline Mat4f vm_mat4f(glm::mat4 const& glm_mat) {
    return vm_mat4f(
        vm_vec4f(glm_mat[0]),
        vm_vec4f(glm_mat[1]),
        vm_vec4f(glm_mat[2]),
        vm_vec4f(glm_mat[3])
    );
}

inline void vm_vec4f(Vec4f& vec, glm::vec4 const& glm_vec) {
    vec.x = glm_vec.x;
    vec.y = glm_vec.y;
    vec.z = glm_vec.z;
    vec.w = glm_vec.w;
}

inline void vm_mat4f(Mat4f& mat, glm::mat4 const& glm_mat) {
    vm_vec4f(mat.cols[0], glm_mat[0]);
    vm_vec4f(mat.cols[1], glm_mat[1]);
    vm_vec4f(mat.cols[2], glm_mat[2]);
    vm_vec4f(mat.cols[3], glm_mat[3]);
}

extern "C" CameraMatrices transforms_create_ortho_camera_matrices(const OthroCameraTransforms transforms);

extern "C" Mat4f transforms_create_2d_model_matrix(Transforms2D transforms);
