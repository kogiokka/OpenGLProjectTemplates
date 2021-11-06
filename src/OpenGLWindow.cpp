#include "OpenGLWindow.hpp"

#include "imgui_impl_sdl.h"
#include <SDL.h>

#include <cstdlib>
#include <iostream>
#include <string>

// Global window
sdl::Window::OpenGLWindow window;

namespace details
{
    void onKeyDownEvent(const SDL_KeyboardEvent& e)
    {
        switch (e.keysym.sym)
        {
            default: break;
        }
    }

    void onKeyUpEvent(const SDL_KeyboardEvent& e)
    {
        switch (e.keysym.sym)
        {
            default: break;
        }
    }

    void onMouseButtonDownEvent(const SDL_MouseButtonEvent& e)
    {
        switch (e.button)
        {
            case SDL_BUTTON_LEFT: break;
            case SDL_BUTTON_RIGHT: break;
            case SDL_BUTTON_MIDDLE: break;
        }
    }

    void onMouseButtonUpEvent(const SDL_MouseButtonEvent& e)
    {
        switch (e.button)
        {
            case SDL_BUTTON_LEFT: break;
            case SDL_BUTTON_RIGHT: break;
            case SDL_BUTTON_MIDDLE: break;
        }
    }

    void onMouseMotionEvent(const SDL_MouseMotionEvent& e)
    {
        switch (e.state)
        {
            case SDL_BUTTON_LMASK: break;
            case SDL_BUTTON_RMASK: break;
        }
    }

    void onWindowEvent(const SDL_WindowEvent& e)
    {
        switch (e.event)
        {
            default: break;
        }
    }
}

namespace sdl::Window::Event
{
    void poll()
    {
        window.events.clear();

        SDL_Event e;
        while (SDL_PollEvent(&e))
            window.events.push_back(e);
    }

    void alwaysProcess(const SDL_Event& event)
    {
        switch (event.type)
        {
            case SDL_QUIT: window.shouldClose = true; break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_q)
                {
                    if (event.key.keysym.mod & KMOD_CTRL)
                    {
                        window.shouldClose = true;
                    }
                }
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    SDL_GetWindowSize(window.handle, &window.size.width, &window.size.height);
                }
                break;
        }
    }

    void process(const SDL_Event& event)
    {
        // ImGui_ImplSDL2_ProcessEvent(&(*it));
        // ImGuiIO const& io = ImGui::GetIO();
        // if (io.WantCaptureKeyboard || io.WantCaptureMouse) return;
        switch (event.type)
        {
            case SDL_KEYDOWN: details::onKeyDownEvent(event.key); break;
            case SDL_KEYUP: details::onKeyUpEvent(event.key); break;
            case SDL_MOUSEBUTTONDOWN: details::onMouseButtonDownEvent(event.button); break;
            case SDL_MOUSEBUTTONUP: details::onMouseButtonUpEvent(event.button); break;
            case SDL_MOUSEMOTION: details::onMouseMotionEvent(event.motion); break;
            case SDL_WINDOWEVENT: details::onWindowEvent(event.window); break;
        }
    }
}

namespace sdl::Window
{
    void create(const std::string& title, int width, int height)
    {
        window.size = {width, height};

        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // OpenGL 3.3
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

#ifdef __linux__
        // Hints for Linux
        SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
#endif

        const Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
        window.handle =
            SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        if (window.handle == nullptr)
        {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            std::exit(EXIT_FAILURE);
        }

        SDL_SetWindowMinimumSize(window.handle, 400, 300);
        window.glContext = SDL_GL_CreateContext(window.handle);
        SDL_GL_MakeCurrent(window.handle, window.glContext);
        SDL_GL_SetSwapInterval(1);
    }

    void destroy() { SDL_DestroyWindow(window.handle); }
    void swap() { SDL_GL_SwapWindow(window.handle); }
}
