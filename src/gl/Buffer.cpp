#include "gl/Buffer.hpp"

namespace gl::Buffer
{
    GLuint create()
    {
        GLuint id;
        glGenBuffers(1, &id);
        return id;
    }

    void destroy(GLuint id) { glDeleteBuffers(1, &id); }
    void bind(GLenum target, GLuint bufferId) { glBindBuffer(target, bufferId); }
    void data(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
    {
        glBufferData(target, size, data, usage);
    }

    void subData(GLenum target, GLintptr offset, GLsizei size, const GLvoid* data)
    {
        glBufferSubData(target, offset, size, data);
    }
}
