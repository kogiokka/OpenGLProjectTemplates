#include "OpenGLWindow.hpp"
#include "gl/Buffer.hpp"
#include "gl/Camera.hpp"
#include "gl/Shader.hpp"
#include "gl/VertexArray.hpp"

#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include <SDL.h>

#include <iostream>
#include <vector>

static std::vector<float> verts = {
    -0.5f,
    -0.5f,
    0.0f, // left
    0.5f,
    -0.5f,
    0.0f, // right
    0.0f,
    0.5f,
    0.0f // top
};

int main(int argc, char* argv[])
{
    sdl::Window::OpenGLWindow window = sdl::Window::create("NTOU SDL2 Beginner Template", 800, 600);

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    std::cout << "OpenGL Version:        " << glGetString(GL_VERSION) << "\n"
              << "GLSL Version:          " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n"
              << "Renderer:              " << glGetString(GL_RENDERER) << "\n"
              << "Vendor:                " << glGetString(GL_VENDOR) << std::endl;

    GLuint vao = gl::VertexArray::create();
    GLuint vbo = gl::Buffer::create();

    const unsigned int floatByteNum = sizeof(float);

    gl::Buffer::bind(GL_ARRAY_BUFFER, vbo);
    gl::Buffer::data(GL_ARRAY_BUFFER, verts.size() * floatByteNum, verts.data(), GL_STATIC_DRAW);

    gl::VertexArray::bind(vao);
    gl::VertexArray::enable(gl::VertexArray::Attrib::Position);

    gl::Buffer::bind(GL_ARRAY_BUFFER, vbo);
    gl::VertexArray::pointer(gl::VertexArray::Attrib::Position, 3, GL_FLOAT, GL_TRUE, 3 * floatByteNum, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window.handle, window.glContext);
    ImGui_ImplOpenGL3_Init();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.FrameBorderSize = 1.0f;

    GLuint program = gl::Shader::create();
    gl::Shader::attach(program, gl::Shader::Stage::Vert, "shaders/default.vert");
    gl::Shader::attach(program, gl::Shader::Stage::Frag, "shaders/default.frag");
    gl::Shader::link(program);

    glm::mat4 const identityMat = glm::mat4(1.0f);
    glm::mat4 const modelMat = identityMat;

    gl::Camera::Viewport = {0, 0, window.size.width, window.size.height};

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 viewDir = glm::vec3(0.0f, 0.0f, -1.0f);

    while (!window.isDone)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window.handle);
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        sdl::Window::handleEvent(window);
        gl::Camera::Viewport.width = window.size.width;
        gl::Camera::Viewport.height = window.size.height;

        auto const& [x, y, w, h] = gl::Camera::Viewport;
        glViewport(x, y, w, h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        const glm::mat4 projMat = gl::Camera::perspective(glm::radians(45.0f), 0.01f, 100.0f);
        const glm::mat4 viewMat = gl::Camera::view(position, position + viewDir);
        gl::Shader::use(program);
        gl::Shader::Uniform::matrix4fv(program, "modelMat", modelMat);
        gl::Shader::Uniform::matrix4fv(program, "viewProjMat", projMat * viewMat);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        sdl::Window::swap(window);
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    gl::Shader::destroy(program);
    sdl::Window::destroy(window);

    SDL_Quit();

    return 0;
}
