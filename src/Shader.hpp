#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

namespace shader
{
    GLuint createProgram();
    void deleteProgram(GLuint programId);
    void attachShader(GLuint programId, GLenum shaderType, std::string const& filepath);
    void linkProgram(GLuint programId);
    void setUniformMatrix4fv(GLuint programId, std::string const& uniformName, glm::mat4 const& mat);
}
