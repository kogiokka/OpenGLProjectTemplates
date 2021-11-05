#pragma once

#include "common.hpp"

#include "glad/glad.h"

namespace gl::Buffer
{
    GLuint create();
    void destroy(GLuint id);
    void bind(BufferTarget target, GLuint vbo);
    void data(BufferTarget target, gl::DataType type, int size, const void* dataPointer, GLenum usage);
}
