#include "gl/Camera.hpp"

namespace gl::Camera
{
    struct Viewport Viewport = {};

    float aspectRatio() { return static_cast<float>(Viewport.width) / static_cast<float>(Viewport.height); }

    glm::mat4 view(glm::vec3 position, glm::vec3 center)
    {
        static const glm::vec3 worldUp { 0.0f, 1.0f, 0.0f };
        return glm::lookAt(position, center, worldUp);
    }

    glm::mat4 orthographic(float fov, float near, float far)
    {
        const float top = fov;
        const float right = aspectRatio() * fov;
        const float bottom = -top;
        const float left = -right;

        return glm::ortho(left, right, bottom, top, near, far);
    }

    glm::mat4 perspective(float fovy, float near, float far)
    {
        return glm::perspective(fovy, aspectRatio(), near, far);
    }
}
