#pragma once
#include <type_traits>

/* Standardy Layout Type
 * https://en.cppreference.com/w/cpp/named_req/StandardLayoutType
 *
 * Make sure the elements are continuous in the memory.
 */
struct Vertex
{
    struct Position
    {
        float x, y, z;
        float* ptr() { return reinterpret_cast<float*>(this); }
    };

    struct Color
    {
        float r, g, b;
        float* ptr() { return reinterpret_cast<float*>(this); }
    };

    Position position;
    Color color;
};

static_assert(std::is_standard_layout_v<Vertex>);
static_assert(std::is_standard_layout_v<Vertex::Position>);
static_assert(std::is_standard_layout_v<Vertex::Color>);
