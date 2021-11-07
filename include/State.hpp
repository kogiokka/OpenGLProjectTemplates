#pragma once

#include "OpenGLWindow.hpp"
#include "World.hpp"

struct State {
    sdl::Window::OpenGLWindow* window;
    World::Scene* scene;
};

extern State state;
