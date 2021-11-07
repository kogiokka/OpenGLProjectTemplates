#pragma once

#include "gl/Vertex.hpp"
#include <vector>

struct World {
    std::vector<Vertex> triangle;

    static void create();
    static void destroy();
};

extern World world;
