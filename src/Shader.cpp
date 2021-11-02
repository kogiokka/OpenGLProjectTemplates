#include "Shader.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace shader::details
{
    bool isCompiled(GLuint shaderObject)
    {
        GLint success;
        glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);

        if (success == GL_TRUE)
        {
            return true;
        }

        GLint logLen;
        std::string log;

        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLen);
        log.resize(logLen);
        glGetShaderInfoLog(shaderObject, logLen, nullptr, log.data());
        std::cerr << "[Error] " << log << std::endl;

        return false;
    }

    bool isLinked(GLuint programId)
    {
        GLint success;
        glGetProgramiv(programId, GL_LINK_STATUS, &success);

        if (success == GL_TRUE)
        {
            return true;
        }

        GLint logLen;
        std::string log;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);
        log.resize(logLen);
        glGetProgramInfoLog(programId, logLen, nullptr, log.data());
        std::cerr << "[Error] " << log << std::endl;

        return false;
    }
}

namespace shader
{
    GLuint createProgram() { return glCreateProgram(); }

    void deleteProgram(GLuint programId) { glDeleteProgram(programId); }

    void attachShader(GLuint programId, GLenum shaderType, std::string const& filepath)
    {
        using namespace std;

        ifstream file;
        string source;

        file.open(filepath.data());
        if (file.fail())
        {
            return;
        }
        source.resize(filesystem::file_size(filepath));
        file.read(source.data(), source.size());
        file.close();

        char const* const shaderSourceArray[1] = {source.c_str()};

        GLuint shaderObject = glCreateShader(shaderType);
        glShaderSource(shaderObject, 1, shaderSourceArray, nullptr);
        glCompileShader(shaderObject);

        if (details::isCompiled(shaderObject))
        {
            glAttachShader(programId, shaderObject);
        }

        glDeleteShader(shaderObject);
    }

    void linkProgram(GLuint programId)
    {
        glLinkProgram(programId);
        if (details::isLinked(programId))
        {
            std::cout << "[Info] Shader objects have been successfully linked into the program!" << std::endl;
        }
        else
        {
            glDeleteProgram(programId);
        }
    }
}
