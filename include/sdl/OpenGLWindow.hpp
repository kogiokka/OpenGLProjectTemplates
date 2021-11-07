#pragma once

#include <SDL.h>
#include <string>
#include <vector>

namespace sdl::Window
{
    struct OpenGLWindow {
        struct Size {
            int width;
            int height;
        };

        bool shouldClose = false;
        SDL_Window* handle = nullptr;
        SDL_GLContext glContext = nullptr;
        Size size = {};
        std::vector<SDL_Event> events = {};
    };

    void create(const std::string& title, int width, int height);
    void destroy();
    void swap();
}

namespace sdl::Window::Event
{
    void poll();
    void process(const SDL_Event& event, bool bypassSceneRoutine);
}

// Global window
extern sdl::Window::OpenGLWindow window;
