#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

namespace gl::shader::get
{
    std::string shaderInfoLog(GLuint shaderObject)
    {
        GLint len;
        std::string log;

        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &len);
        log.resize(len);
        glGetShaderInfoLog(shaderObject, len, nullptr, log.data());
        return log;
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

    bool compileStatus(GLuint shaderObject)
    {
        GLint success;
        glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);

        if (success == GL_TRUE)
        {
            return true;
        }

        std::cerr << "[Error] " << shaderInfoLog(shaderObject) << std::endl;
        return false;
    }

    bool linkStatus(GLuint programId)
    {
        GLint success;
        glGetProgramiv(programId, GL_LINK_STATUS, &success);

        if (success == GL_TRUE)
        {
            return true;
        }

        std::cerr << "[Error] " << programInfoLog(programId) << std::endl;
        return false;
    }
}

namespace gl::shader::uniform
{
    namespace details
    {
        int getLocation(GLuint programId, std::string const uniformName)
        {
            using namespace std;

            static unordered_map<string, GLint> locations;

            if (locations.find(uniformName) != locations.end())
            {
                return locations[uniformName];
            }

            GLchar const* name = uniformName.c_str();
            int loc = glGetUniformLocation(programId, name);
            if (loc == -1)
            {
                cerr << "[Error] Uniform \"" << uniformName << "\" does not exist." << endl;
            }

            locations[uniformName] = loc;

            return loc;
        }
    }

    void matrix4fv(GLuint programId, std::string const& uniformName, glm::mat4 const& mat)
    {
        glUniformMatrix4fv(details::getLocation(programId, uniformName), 1, GL_FALSE, glm::value_ptr(mat));
    }
}

namespace gl::shader
{
    GLuint create() { return glCreateProgram(); }
    void destroy(GLuint programId) { glDeleteProgram(programId); }
    void use(GLuint programId) { glUseProgram(programId); }
    void link(GLuint programId)
    {
        glLinkProgram(programId);
        if (get::linkStatus(programId))
        {
            std::cout << "[Info] Shader objects have been successfully linked into the program!" << std::endl;
        }
        else
        {
            glDeleteProgram(programId);
        }
    }

    void attach(GLuint programId, Stage stage, std::string const& filepath)
    {
        using namespace std;

        ifstream file;
        string source = "";

        file.open(filepath.data());
        if (file.fail())
        {
            cerr << "Failed to read shader file: \"" << filepath << "\"." << endl;
            return;
        }
        source.resize(filesystem::file_size(filepath));
        file.read(source.data(), source.size());
        file.close();

        ::gl::shader::attachSource(programId, stage, source);
    }

    void attachSource(GLuint programId, Stage stage, std::string const& source)
    {
        char const* const shaderSourceArray[1] = {source.c_str()};

        GLuint shaderObject = glCreateShader(stage);
        glShaderSource(shaderObject, 1, shaderSourceArray, nullptr);
        glCompileShader(shaderObject);

        if (get::compileStatus(shaderObject))
        {
            glAttachShader(programId, shaderObject);
        }

        glDeleteShader(shaderObject);
    }
}
