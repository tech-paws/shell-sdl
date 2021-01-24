#include "primitives.hpp"
#include "transforms.hpp"
#include <glm/gtc/matrix_transform.hpp>

extern "C" CameraMatrices transforms_create_ortho_camera_matrices(const OthroCameraTransforms transforms) {
    glm::vec3 eye = glm::vec3(glm_vec2(transforms.position), 1.0f);
    glm::vec3 target = glm::vec3(glm_vec2(transforms.position), 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 model_matrix;
    glm::mat4 mvp_matrix;

    view_matrix = glm::lookAt(eye, target, up);
    projection_matrix = glm::ortho(
        0.0f, transforms.viewport_size.x,
        0.0f, transforms.viewport_size.y,
        0.0f, 10.0f
    );

    if (transforms.zoom > 1.0f) {
        model_matrix = glm::scale(
            glm::mat4(),
            glm::vec3(transforms.zoom, transforms.zoom, 1.0f)
        );
    } else {
        model_matrix = glm::mat4(1);
    }

    mvp_matrix = projection_matrix * model_matrix * view_matrix;

    CameraMatrices camera_matrices;

    vm_mat4f(camera_matrices.view_matrix, view_matrix);
    vm_mat4f(camera_matrices.projection_matrix, projection_matrix);
    vm_mat4f(camera_matrices.model_matrix, model_matrix);
    vm_mat4f(camera_matrices.mvp_matrix, mvp_matrix);

    return camera_matrices;
}

extern "C" Mat4f transforms_create_2d_model_matrix(Transforms2D transforms) {
    const auto translate_matrix = glm::translate(
        glm::mat4(1),
        glm::vec3(glm_vec2(transforms.position), 0.0f)
    );

    const auto rotate_matrix = glm::rotate(
        glm::mat4(1),
        transforms.rotation,
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    const auto scale_matrix = glm::scale(
        glm::mat4(1),
        glm::vec3(glm_vec2(transforms.scaling), 1.0f)
    );

    return vm_mat4f(translate_matrix * rotate_matrix * scale_matrix);
}
