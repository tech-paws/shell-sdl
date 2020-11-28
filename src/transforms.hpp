#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

struct CameraMatrices {
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;
    glm::mat4 mvpMatrix;
};

struct OthroCameraTransforms {
    glm::vec2 viewportSize;
    glm::vec2 position;
    float zoom;
};

struct Transforms2D {
    glm::vec2 position;
    glm::vec2 scaling;
    float rotation { 0.f };
};

CameraMatrices gapiCreateOrthoCameraMatrices(const OthroCameraTransforms transforms);

glm::mat4 gapiCreate2DModelMatrix(Transforms2D transforms);
