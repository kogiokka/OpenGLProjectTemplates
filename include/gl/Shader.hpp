#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

namespace gl::Shader
{
    enum Stage : GLenum {
        Vert = GL_VERTEX_SHADER,
        Tesc = GL_TESS_CONTROL_SHADER,
        Tese = GL_TESS_EVALUATION_SHADER,
        Geom = GL_GEOMETRY_SHADER,
        Frag = GL_FRAGMENT_SHADER,
        Comp = GL_COMPUTE_SHADER,
    };

    GLuint create();
    void destroy(GLuint programId);
    void link(GLuint programId);
    void use(GLuint programId);
    void attach(GLuint programId, Stage stage, const std::string& filepath);
    void attachSource(GLuint programId, Stage stage, const std::string& source);
}

namespace gl::Shader::Uniform
{
    void matrix4fv(GLuint programId, const std::string& uniformName, const glm::mat4& mat);
}
