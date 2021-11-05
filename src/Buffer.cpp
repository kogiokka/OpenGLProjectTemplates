#include "Buffer.hpp"

namespace gl::Buffer
{
    GLuint create()
    {
        GLuint id;
        glGenBuffers(1, &id);
        return id;
    }

    void destroy(GLuint id) { glDeleteBuffers(1, &id); }
    void bind(BufferTarget target, GLuint bufferId) { glBindBuffer(target, bufferId); }
    void data(BufferTarget target, gl::DataType type, int size, const void* dataPointer, GLenum usage)
    {
        glBufferData(target, size * gl::sizeOf(type), dataPointer, usage);
    }
}
