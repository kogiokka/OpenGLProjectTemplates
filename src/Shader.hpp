#pragma once

#include <glad/glad.h>

#include <string>

namespace shader
{
    GLuint createProgram();
    void deleteProgram(GLuint programId);
    void attachShader(GLuint programId, GLenum shaderType, std::string const& filepath);
    void linkProgram(GLuint programId);
}
