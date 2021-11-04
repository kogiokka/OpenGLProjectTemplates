#include "OpenGLWindow.hpp"
#include "Shader.hpp"

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

GLuint initShaderProgram()
{
    using namespace shader;

    GLuint program = createProgram();

    attachShader(program, GL_VERTEX_SHADER, "shaders/default.vert");
    attachShader(program, GL_FRAGMENT_SHADER, "shaders/default.frag");
    linkProgram(program);

    return program;
}

int main(int argc, char* argv[])
{
    sdl::window::OpenGLWindow window = sdl::window::init("NTOU SDL2 Beginner Template", 800, 600);

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    std::cout << "OpenGL Version:        " << glGetString(GL_VERSION) << "\n"
              << "GLSL Version:          " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n"
              << "Renderer:              " << glGetString(GL_RENDERER) << "\n"
              << "Vendor:                " << glGetString(GL_VENDOR) << std::endl;

    GLuint vao;
    GLuint vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

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

    GLuint const program = initShaderProgram();

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

        sdl::window::handleEvent(window);

        auto const& [x, y, w, h] = window.viewport;
        glViewport(x, y, w, h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);
        glUseProgram(program);

        shader::setUniformMatrix4fv(program, "modelMat", modelMatrix);
        shader::setUniformMatrix4fv(program, "viewProjMat", orthoProjectionMatrix(w, h, 1.0f) * viewMatrix);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        sdl::window::swap(window);
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    shader::deleteProgram(program);
    sdl::window::destroy(window);

    SDL_Quit();

    return 0;
}
