#include "World.hpp"

World::Scene scene;

namespace World
{
    void create()
    {
        scene.triangle = {{
            Vertex{
                {-0.5f, -0.5f, 0.0f},
                {1.0f, 0.0f, 0.0f},
            },
            Vertex{
                {0.5f, -0.5f, 0.0f},
                {0.0f, 1.0f, 0.0f},
            },
            Vertex{
                {0.0f, 0.5f, 0.0f},
                {0.0f, 0.0f, 1.0f},
            },
        }};
    }

    void destroy() { scene.triangle.clear(); }
}
