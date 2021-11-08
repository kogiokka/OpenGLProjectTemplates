#pragma once
#include "gl/Vertex.hpp"

#include <glm/glm.hpp>

#include <array>

struct Triangle {
    std::array<Vertex, 3> vertices;
    unsigned int count() { return vertices.size(); }
    Vertex* data() { return vertices.data(); }
};

struct World {
    Triangle* triangle;
    glm::vec3 skyColor;

    void create();
    void destroy();
};

// Global world
extern World world;
