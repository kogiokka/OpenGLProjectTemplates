#pragma once

#include "gl/Vertex.hpp"

#include <array>

struct Triangle {
    std::array<Vertex, 3> vertices;
};

struct World {
    Triangle* triangle;

    void create();
    void destroy();
};

// Global world
extern World world;
