#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

struct CameraMatrices {
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 model_matrix;
    glm::mat4 mvp_matrix;
};

struct OthroCameraTransforms {
    glm::vec2 viewport_size;
    glm::vec2 position;
    float zoom;
};

struct Transforms2D {
    glm::vec2 position;
    glm::vec2 scaling;
    float rotation { 0.f };
};

CameraMatrices transforms_create_ortho_camera_matrices(const OthroCameraTransforms transforms);

glm::mat4 transforms_create_2d_model_matrix(Transforms2D transforms);
