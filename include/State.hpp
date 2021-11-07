#pragma once

#define EXAMPLE_VERSION "1.10"

#include "World.hpp"
#include "sdl/OpenGLWindow.hpp"

struct State {
    sdl::Window::OpenGLWindow* window;
    World* world;
};

extern State state;
