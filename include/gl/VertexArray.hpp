#pragma once

#include "common.hpp"

#include "glad/glad.h"

namespace gl::VertexArray
{
    GLuint create();
    void destroy(GLuint id);
    void bind(GLuint id);
    void enable(VertexAttrib index);
    void disable(VertexAttrib index);
    void pointer(VertexAttrib index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLint pointer);
}
