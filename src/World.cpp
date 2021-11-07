#include "World.hpp"

// Global world
World world;

void World::create()
{
    world.triangle = new Triangle {
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
    delete world.triangle;
}
