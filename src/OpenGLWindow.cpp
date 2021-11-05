#include "OpenGLWindow.hpp"

#include "imgui_impl_sdl.h"
#include <SDL.h>

#include <cstdlib>
#include <iostream>
#include <string>

namespace sdl::Window::details
{
    void onWindowEvent(OpenGLWindow& window, SDL_WindowEvent const& e)
    {
        switch (e.event)
        {
            case SDL_WINDOWEVENT_RESIZED:
                SDL_GetWindowSize(window.handle, &window.size.width, &window.size.height);
                break;
            default: break;
        }
    }

    void onKeyDownEvent(OpenGLWindow& window, SDL_KeyboardEvent const& e)
    {
        switch (e.keysym.sym)
        {
            case SDLK_q:
                if (KMOD_CTRL & e.keysym.mod) window.isDone = true;
                break;
        }
    }

    void onMouseMotionEvent(OpenGLWindow& window, SDL_MouseMotionEvent const& e) { }
}

namespace sdl::Window
{
    OpenGLWindow create(std::string const& title, int width, int height)
    {
        OpenGLWindow window;
        window.size = {800, 600};

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

        Uint32 const flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
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

        return window;
    }

    void destroy(OpenGLWindow& window) { SDL_DestroyWindow(window.handle); }
    void swap(OpenGLWindow& window) { SDL_GL_SwapWindow(window.handle); }
    void handleEvent(OpenGLWindow& window)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            ImGuiIO const& io = ImGui::GetIO();
            if (io.WantCaptureKeyboard || io.WantCaptureMouse) continue;

            switch (event.type)
            {
                case SDL_QUIT: window.isDone = true; break;
                case SDL_KEYDOWN: details::onKeyDownEvent(window, event.key); break;
                case SDL_KEYUP: break;
                case SDL_MOUSEBUTTONDOWN: break;
                case SDL_MOUSEBUTTONUP: break;
                case SDL_MOUSEMOTION: details::onMouseMotionEvent(window, event.motion); break;
                case SDL_WINDOWEVENT: details::onWindowEvent(window, event.window); break;
            }
        }
    }
}
