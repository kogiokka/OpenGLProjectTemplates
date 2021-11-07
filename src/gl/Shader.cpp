#include "gl/Shader.hpp"

#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

namespace gl::Shader::Uniform
{
    namespace details
    {
        int getLocation(GLuint programId, const std::string& uniformName)
        {
            using namespace std;

            static unordered_map<string, GLint> locations;

            if (locations.find(uniformName) != locations.end()) {
                return locations[uniformName];
            }

            const GLchar* name = uniformName.c_str();
            int loc = glGetUniformLocation(programId, name);
            if (loc == -1) {
                cerr << "[Error] Uniform \"" << uniformName << "\" does not exist." << endl;
            }

            locations[uniformName] = loc;

            return loc;
        }
    }

    void matrix4fv(GLuint programId, const std::string& uniformName, const glm::mat4& mat)
    {
        glUniformMatrix4fv(details::getLocation(programId, uniformName), 1, GL_FALSE, glm::value_ptr(mat));
    }
}

namespace gl::Shader::Param
{
    GLint compileStatus(GLuint shaderObject)
    {
        GLint status;
        glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
        return status;
    }

    std::string shaderInfoLog(GLuint shaderObject)
    {
        GLint len;
        std::string log;

        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &len);
        log.resize(len);
        glGetShaderInfoLog(shaderObject, len, nullptr, log.data());
        return log;
    }

    GLint linkStatus(GLuint programId)
    {
        GLint status;
        glGetProgramiv(programId, GL_LINK_STATUS, &status);
        return status;
    }

    std::string programInfoLog(GLuint programId)
    {
        GLint len;
        std::string log;

        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &len);
        log.resize(len);
        glGetProgramInfoLog(programId, len, nullptr, log.data());
        return log;
    }

    GLint attachedShaders(GLuint programId)
    {
        GLint num;
        glGetProgramiv(programId, GL_ATTACHED_SHADERS, &num);
        return num;
    }

    GLint activeAttributes(GLuint programId)
    {
        GLint num;
        glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &num);
        return num;
    }
}

namespace gl::Shader
{
    GLuint create() { return glCreateProgram(); }
    void destroy(GLuint programId) { glDeleteProgram(programId); }
    void use(GLuint programId) { glUseProgram(programId); }
    void link(GLuint programId)
    {
        glLinkProgram(programId);
        if (Param::linkStatus(programId) == GL_TRUE) {
            std::cout << "[Info] Linking Shader object(s) succeeded!" << std::endl;
            std::cout << "[Info] Number of attached shaders: " << Param::attachedShaders(programId) << std::endl;
            std::cout << "[Info] Number of active attributes: " << Param::activeAttributes(programId) << std::endl;
        } else {
            std::cerr << "[Error] " << Param::programInfoLog(programId) << std::endl;
            glDeleteProgram(programId);
        }
    }

    void attach(GLuint programId, Stage stage, const std::string& filepath)
    {
        using namespace std;

        ifstream file;
        string source = "";

        file.open(filepath.data());
        if (file.fail()) {
            cerr << "Failed to read shader file: \"" << filepath << "\"." << endl;
            return;
        }
        source.resize(filesystem::file_size(filepath));
        file.read(source.data(), source.size());
        file.close();

        ::gl::Shader::attachSource(programId, stage, source);
    }

    void attachSource(GLuint programId, Stage stage, const std::string& source)
    {
        const char* const shaderSourceArray[1] = { source.c_str() };

        GLuint shaderObject = glCreateShader(stage);
        glShaderSource(shaderObject, 1, shaderSourceArray, nullptr);
        glCompileShader(shaderObject);

        if (Param::compileStatus(shaderObject) == GL_TRUE) {
            glAttachShader(programId, shaderObject);
        } else {
            std::cerr << "[Error] " << Param::shaderInfoLog(shaderObject) << std::endl;
        }

        glDeleteShader(shaderObject);
    }
}
