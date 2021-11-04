#pragma once

#include <SDL.h>
#include <string>

namespace sdl::window
{
    struct OpenGLWindow
    {
        SDL_Window* handle = nullptr;
        SDL_GLContext glContext = nullptr;
        SDL_Rect viewport = {};
        bool isDone = false;
    };
    OpenGLWindow init(std::string const& title, int width, int height);
    void destroy(OpenGLWindow& window);
    void swap(OpenGLWindow& window);
    void handleEvent(OpenGLWindow& window);
}
