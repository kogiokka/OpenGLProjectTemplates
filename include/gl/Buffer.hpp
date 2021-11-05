#pragma once

#include "common.hpp"

#include "glad/glad.h"

namespace gl::Buffer
{
    GLuint create();
    void destroy(GLuint id);
    void bind(GLenum target, GLuint vbo);
    void data(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
}
