#include "State.hpp"
#include "UI.hpp"

#include "gl/Buffer.hpp"
#include "gl/Camera.hpp"
#include "gl/Shader.hpp"
#include "gl/Vertex.hpp"
#include "gl/VertexArray.hpp"

#include "glad/glad.h"
#include <SDL.h>

#include <iostream>
#include <vector>

State state;

int main(int argc, char* argv[])
{
    sdl::Window::create("NTOU SDL2 Beginner Template", 1200, 800);
    state.window = &window;

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    std::cout << "OpenGL Version:        " << glGetString(GL_VERSION) << "\n"
              << "GLSL Version:          " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n"
              << "Renderer:              " << glGetString(GL_RENDERER) << "\n"
              << "Vendor:                " << glGetString(GL_VENDOR) << std::endl;

    World::create();
    state.scene = &scene;

    GLuint vao = gl::VertexArray::create();
    GLuint vbo = gl::Buffer::create();

    gl::Buffer::bind(GL_ARRAY_BUFFER, vbo);
    gl::Buffer::data(GL_ARRAY_BUFFER,
                     state.scene->triangle.size() * sizeof(Vertex),
                     state.scene->triangle.data(),
                     GL_DYNAMIC_DRAW);

    using gl::VertexArray::Attrib;

    gl::VertexArray::bind(vao);
    gl::VertexArray::enable(Attrib::Position);
    gl::VertexArray::enable(Attrib::Color);

    gl::Buffer::bind(GL_ARRAY_BUFFER, vbo);
    gl::VertexArray::pointer(Attrib::Position, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), offsetof(Vertex, position));
    gl::VertexArray::pointer(Attrib::Color, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), offsetof(Vertex, color));

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    GLuint program = gl::Shader::create();
    gl::Shader::attach(program, gl::Shader::Stage::Vert, "shaders/default.vert");
    gl::Shader::attach(program, gl::Shader::Stage::Frag, "shaders/default.frag");
    gl::Shader::link(program);

    const glm::mat4 identityMat = glm::mat4(1.0f);
    const glm::mat4 modelMat = identityMat;

    gl::Camera::Viewport = {0, 0, state.window->size.width, state.window->size.height};

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 viewDir = glm::vec3(0.0f, 0.0f, -1.0f);

    UI::create();

    while (!state.window->shouldClose)
    {
        sdl::Window::Event::poll();

        for (auto it = std::cbegin(state.window->events); it != std::cend(state.window->events); it++)
        {
            sdl::Window::Event::alwaysProcess(*it);
            UI::processEvent(*it);
            if (UI::Var.GrabbingEvent) continue;
            sdl::Window::Event::process(*it);
        }

        gl::Camera::Viewport.width = state.window->size.width;
        gl::Camera::Viewport.height = state.window->size.height;

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
        UI::render();

        sdl::Window::swap();
    }

    World::destroy();
    UI::destroy();

    gl::Shader::destroy(program);
    gl::Buffer::destroy(vbo);
    gl::VertexArray::destroy(vao);
    sdl::Window::destroy();

    SDL_Quit();

    return 0;
}
