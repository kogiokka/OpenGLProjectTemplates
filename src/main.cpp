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
#include <type_traits> // std::is_standard_layout_v
#include <vector>

/* Standardy Layout Type
 * https://en.cppreference.com/w/cpp/named_req/StandardLayoutType
 *
 * Make sure the elements are continuous in the memory.
 */
struct Vertex
{
    struct Position
    {
        float x, y, z;
        float* ptr() { return reinterpret_cast<float*>(this); }
    };

    struct Color
    {
        float r, g, b;
        float* ptr() { return reinterpret_cast<float*>(this); }
    };

    Position position;
    Color color;
};

static_assert(std::is_standard_layout_v<Vertex>);
static_assert(std::is_standard_layout_v<Vertex::Position>);
static_assert(std::is_standard_layout_v<Vertex::Color>);

static std::vector<Vertex> triangle = {
    Vertex{
        {-0.5f, -0.5f, 0.0f},
        {1.0f, 0.0f, 0.0f},
    },
    Vertex{
        {0.5f, -0.5f, 0.0f},
        {0.0f, 1.0f, 0.0f},
    },
    Vertex{
        {0.0f, 0.5f, 0.0f},
        {0.0f, 0.0f, 1.0f},
    },
};

void paintGUI(sdl::Window::OpenGLWindow& window)
{
    static bool show_VertexEditor = false;
    static bool flag_NoBackground = true;
    ImGuiWindowFlags flags = 0;

    if (flag_NoBackground) flags |= ImGuiWindowFlags_NoBackground;

#ifndef NDEBUG
    static bool show_ImGuiDemo = false;
#endif

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window.handle);

    ImGui::NewFrame();

#ifndef NDEBUG
    if (show_ImGuiDemo)
    {
        ImGui::ShowDemoWindow(&show_ImGuiDemo);
    }
#endif

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::BeginMenu("Preferences"))
            {
                ImGui::MenuItem("ImGui Window", nullptr, false, false);
                ImGui::Checkbox("No background", &flag_NoBackground);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Vertex Editor", nullptr, &show_VertexEditor);
#ifndef NDEBUG
            ImGui::MenuItem("Dear ImGui Demo", nullptr, &show_ImGuiDemo);
#endif
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (show_VertexEditor)
    {
        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        ImGui::Begin("Vertex Editor", &show_VertexEditor, flags);
        if (ImGui::BeginTabBar("##vertex-editor-tabs"))
        {
            if (ImGui::BeginTabItem("Color"))
            {
                for (std::size_t i = 0; i < triangle.size(); i++)
                {
                    Vertex::Color& color = triangle[i].color;
                    const GLintptr offset = i * sizeof(Vertex) + 1 * sizeof(Vertex::Position);
                    const GLsizei size = sizeof(Vertex::Color);

                    const std::string name = "Vertex " + std::to_string(i);
                    if (ImGui::ColorEdit3(name.c_str(), triangle[i].color.ptr()))
                    {
                        gl::Buffer::subData(GL_ARRAY_BUFFER, offset, size, color.ptr());
                    }
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Position"))
            {
                for (std::size_t i = 0; i < triangle.size(); i++)
                {
                    Vertex::Position& pos = triangle[i].position;
                    const GLintptr offset = i * sizeof(Vertex) + 0 * sizeof(Vertex::Position);
                    const GLsizei size = sizeof(Vertex::Position);

                    const std::string name = "Vertex " + std::to_string(i);
                    if (ImGui::SliderFloat2(name.c_str(), pos.ptr(), -1.50f, 1.50f))
                    {
                        gl::Buffer::subData(GL_ARRAY_BUFFER, offset, size, pos.ptr());
                    }
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    ImGui::Render(); // End of new frame

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

    gl::Buffer::bind(GL_ARRAY_BUFFER, vbo);
    gl::Buffer::data(GL_ARRAY_BUFFER, triangle.size() * sizeof(Vertex), triangle.data(), GL_DYNAMIC_DRAW);

    using gl::VertexArray::Attrib;

    gl::VertexArray::bind(vao);
    gl::VertexArray::enable(Attrib::Position);
    gl::VertexArray::enable(Attrib::Color);

    gl::Buffer::bind(GL_ARRAY_BUFFER, vbo);
    gl::VertexArray::pointer(Attrib::Position, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), offsetof(Vertex, position));
    gl::VertexArray::pointer(Attrib::Color, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), offsetof(Vertex, color));

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

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

    const glm::mat4 identityMat = glm::mat4(1.0f);
    const glm::mat4 modelMat = identityMat;

    gl::Camera::Viewport = {0, 0, window.size.width, window.size.height};

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 viewDir = glm::vec3(0.0f, 0.0f, -1.0f);

    while (!window.isDone)
    {
        sdl::Window::handleEvent(window);
        gl::Camera::Viewport.width = window.size.width;
        gl::Camera::Viewport.height = window.size.height;

        const auto& [x, y, w, h] = gl::Camera::Viewport;
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
        paintGUI(window);

        sdl::Window::swap(window);
    }

    gl::Shader::destroy(program);
    gl::Buffer::destroy(vbo);
    gl::VertexArray::destroy(vao);
    sdl::Window::destroy(window);

    SDL_Quit();

    return 0;
}
