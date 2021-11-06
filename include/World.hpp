#pragma once

#include "gl/Vertex.hpp"
#include <vector>

namespace World
{
    struct Scene
    {
        std::vector<Vertex> triangle;
    };

    void create();
    void destroy();
}

extern World::Scene scene;
