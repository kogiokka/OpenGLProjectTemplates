#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gl::Camera
{
    struct Viewport {
        int x;
        int y;
        int width;
        int height;
    };

    extern struct Viewport Viewport;

    float aspectRatio();
    glm::mat4 view(glm::vec3 position, glm::vec3 center);
    glm::mat4 orthographic(float fov, float near, float far);
    glm::mat4 perspective(float fovy, float near, float far);
}
