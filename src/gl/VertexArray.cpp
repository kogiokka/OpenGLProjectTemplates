#include "gl/VertexArray.hpp"

namespace gl::VertexArray
{
    GLuint create()
    {
        GLuint id;
        glGenVertexArrays(1, &id);
        return id;
    }
    void destroy(GLuint id) { glDeleteVertexArrays(1, &id); }
    void bind(GLuint id) { glBindVertexArray(id); }
    void enable(Attrib index) { glEnableVertexAttribArray(index); }
    void disable(Attrib index) { glDisableVertexAttribArray(index); }
    void pointer(Attrib index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLint offset)
    {
        glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<const void*>(offset));
    }
}
