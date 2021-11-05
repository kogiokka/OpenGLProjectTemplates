#pragma once

#include <SDL.h>
#include <string>

namespace sdl::Window
{
    struct OpenGLWindow
    {
        struct Size
        {
            int width;
            int height;
        };

        SDL_Window* handle = nullptr;
        SDL_GLContext glContext = nullptr;
        Size size;
        bool isDone = false;
    };
    OpenGLWindow create(std::string const& title, int width, int height);
    void destroy(OpenGLWindow& window);
    void swap(OpenGLWindow& window);
    void handleEvent(OpenGLWindow& window);
}
