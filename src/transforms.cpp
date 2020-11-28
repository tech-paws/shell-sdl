#include "transforms.hpp"
#include <glm/gtc/matrix_transform.hpp>

CameraMatrices gapiCreateOrthoCameraMatrices(const OthroCameraTransforms transforms) {
    glm::vec3 eye = glm::vec3(transforms.position, 1.0f);
    glm::vec3 target = glm::vec3(transforms.position, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    CameraMatrices cameraMatrices;

    cameraMatrices.viewMatrix = glm::lookAt(eye, target, up);
    cameraMatrices.projectionMatrix = glm::ortho(
        0.0f, transforms.viewportSize.x,
        0.0f, transforms.viewportSize.y,
        0.0f, 10.0f
    );

    if (transforms.zoom > 1.0f) {
        cameraMatrices.modelMatrix = glm::scale(
            glm::mat4(),
            glm::vec3(transforms.zoom, transforms.zoom, 1.0f)
        );
    } else {
        cameraMatrices.modelMatrix = glm::mat4(1);
    }

    cameraMatrices.mvpMatrix =
        cameraMatrices.projectionMatrix *
        cameraMatrices.modelMatrix *
        cameraMatrices.viewMatrix;

    return cameraMatrices;
}

glm::mat4 gapiCreate2DModelMatrix(Transforms2D transforms) {
    const auto translateMatrix = glm::translate(
        glm::mat4(1),
        glm::vec3(transforms.position, 0.0f)
    );

    const auto rotateMatrix = glm::rotate(
        glm::mat4(1),
        transforms.rotation,
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    const auto scaleMatrix = glm::scale(
        glm::mat4(1),
        glm::vec3(transforms.scaling, 1.0f)
    );

    return translateMatrix * rotateMatrix * scaleMatrix;
}
