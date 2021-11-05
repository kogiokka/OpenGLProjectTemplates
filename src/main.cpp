#include "OpenGLWindow.hpp"
#include "gl/Buffer.hpp"
#include "gl/Shader.hpp"
#include "gl/VertexArray.hpp"

#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

float aspectRatio(int width, int height) { return static_cast<float>(width) / static_cast<float>(height); }

glm::mat4 orthoProjectionMatrix(int width, int height, float fov)
{
    float const top = fov;
    float const right = aspectRatio(width, height) * fov;
    float const bottom = -top;
    float const left = -right;

    return glm::ortho(left, right, bottom, top, 0.0f, 10.0f);
}

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

    gl::Buffer::bind(gl::BufferTarget::Attribs, vbo);
    gl::Buffer::data(gl::BufferTarget::Attribs, gl::DataType::Float, verts.size(), verts.data(), GL_STATIC_DRAW);

    gl::VertexArray::bind(vao);
    gl::VertexArray::enable(gl::VertexAttrib::Position);

    gl::Buffer::bind(gl::BufferTarget::Attribs, vbo);
    gl::VertexArray::pointer(gl::VertexAttrib::Position, gl::DataType::Float, 3, 3, 0);

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
    glm::mat4 const modelMatrix = identityMat;
    glm::mat4 const viewMatrix = identityMat;

    while (!window.isDone)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window.handle);
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        sdl::Window::handleEvent(window);

        auto const& [x, y, w, h] = window.viewport;
        glViewport(x, y, w, h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        gl::Shader::use(program);
        gl::Shader::Uniform::matrix4fv(program, "modelMat", modelMatrix);
        gl::Shader::Uniform::matrix4fv(program, "viewProjMat", orthoProjectionMatrix(w, h, 1.0f) * viewMatrix);

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
