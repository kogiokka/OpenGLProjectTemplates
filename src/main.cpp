#include "State.hpp"
#include "UI.hpp"

#include "gl/Buffer.hpp"
#include "gl/Camera.hpp"
#include "gl/Shader.hpp"
#include "gl/Vertex.hpp"
#include "gl/VertexArray.hpp"

#include <SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <vector>

State state;

void init();
void update();
void destroy();

int main(int argc, char* argv[])
{
    init();

    std::cout << "OpenGL Version:        " << glGetString(GL_VERSION) << "\n"
              << "GLSL Version:          " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n"
              << "Renderer:              " << glGetString(GL_RENDERER) << "\n"
              << "Vendor:                " << glGetString(GL_VENDOR) << std::endl;

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    GLuint vao = gl::VertexArray::create();
    GLuint vbo = gl::Buffer::create();
    GLuint program = gl::Shader::create();

    using gl::VertexArray::Attrib;

    gl::VertexArray::bind(vao);
    gl::VertexArray::enable(Attrib::Position);
    gl::VertexArray::enable(Attrib::Color);

    const auto& verts = state.world->triangle->vertices;
    gl::Buffer::bind(GL_ARRAY_BUFFER, vbo);
    gl::Buffer::data(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_DYNAMIC_DRAW);
    gl::VertexArray::pointer(Attrib::Position, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), offsetof(Vertex, position));
    gl::VertexArray::pointer(Attrib::Color, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), offsetof(Vertex, color));

    gl::Shader::attach(program, gl::Shader::Stage::Vert, "shaders/default.vert");
    gl::Shader::attach(program, gl::Shader::Stage::Frag, "shaders/default.frag");
    gl::Shader::link(program);

    const glm::mat4 identityMat = glm::mat4(1.0f);
    const glm::mat4 modelMat = identityMat;

    gl::Camera::Viewport = { 0, 0, state.window->size.width, state.window->size.height };

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 viewDir = glm::vec3(0.0f, 0.0f, -1.0f);

    while (!state.window->shouldClose) {
        update();

        gl::Camera::Viewport.width = state.window->size.width;
        gl::Camera::Viewport.height = state.window->size.height;

        const auto& [x, y, w, h] = gl::Camera::Viewport;
        const glm::vec3& clearColor = state.world->skyColor;
        glViewport(x, y, w, h);
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        const glm::mat4 projMat = gl::Camera::perspective(glm::radians(45.0f), 0.01f, 100.0f);
        const glm::mat4 viewMat = gl::Camera::view(position, position + viewDir);
        gl::Shader::use(program);
        gl::Shader::Uniform::matrix4fv(program, "modelMat", modelMat);
        gl::Shader::Uniform::matrix4fv(program, "viewProjMat", projMat * viewMat);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        ui::render();

        sdl::Window::swap();
    }

    gl::Shader::destroy(program);
    gl::Buffer::destroy(vbo);
    gl::VertexArray::destroy(vao);

    return 0;
}

void init()
{
    state.window = &window;
    state.world = &world;

    sdl::Window::create("NTOU SDL2 Beginner Template", 1200, 800);
    ui::create();

    state.world->create();
}

void update()
{
    sdl::Window::Event::poll();
    for (auto it = std::cbegin(state.window->events); it != std::cend(state.window->events); it++) {
        ui::Event::process(*it);
        sdl::Window::Event::process(*it, ui::Var.WantCaptureEvent);
    }
}

void destroy()
{
    state.world->destroy();

    ui::destroy();
    sdl::Window::destroy();
}
