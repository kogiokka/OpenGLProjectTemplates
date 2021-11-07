#pragma once

#include "glad/glad.h"

namespace gl::VertexArray
{
    enum Attrib : int {
        Position = 0,
        Color,
    };

    GLuint create();
    void destroy(GLuint id);
    void bind(GLuint id);
    void enable(Attrib index);
    void disable(Attrib index);
    void pointer(Attrib index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLint pointer);
}
