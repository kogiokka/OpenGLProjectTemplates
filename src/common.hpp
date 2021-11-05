#pragma once

#include "glad/glad.h"

namespace gl
{
    enum VertexAttrib : GLuint
    {
        Position = 0,
        Color,
    };

    enum BufferTarget : GLenum
    {
        Attribs = GL_ARRAY_BUFFER,
        Indices = GL_ELEMENT_ARRAY_BUFFER,
    };

    enum DataType : GLenum
    {
        Short = GL_SHORT,
        Int = GL_INT,
        Float = GL_FLOAT,
        Double = GL_DOUBLE,
        UnsignedShort = GL_UNSIGNED_SHORT,
        UnsignedInt = GL_UNSIGNED_INT,
    };

    unsigned int sizeOf(DataType type);
}
