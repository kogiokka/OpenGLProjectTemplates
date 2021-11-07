#include "World.hpp"

World world;

void World::create()
{
    world.triangle = {
        Vertex {
            { -0.5f, -0.5f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
        },
        Vertex {
            { 0.5f, -0.5f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
        },
        Vertex {
            { 0.0f, 0.5f, 0.0f },
            { 0.0f, 0.0f, 1.0f },
        },
    };
}

void World::destroy()
{
    world.triangle.clear();
}
