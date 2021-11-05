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
    void pointer(VertexAttrib index, gl::DataType type, int attribSize, int vertexSize, const void* offset);
}
