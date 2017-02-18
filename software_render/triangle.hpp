#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <algorithm>

#include "geometry/geometry.hpp"
#include "common.hpp"

namespace render
{

inline void triangle(std::array<Vec2i, 3> vertexes, sdl_texture& image, const uint32_t& color)
{
    Vec2i center = (vertexes[0] + vertexes[1] + vertexes[2])*0.3333f;
    std::sort(vertexes.begin(), vertexes.end(), [&center](const Vec2i& l, const Vec2i& r)->bool {return clockwise_less(l,r,center);});

    std::array<Vec2i, 3> sizes = {{
                                      vabs(vertexes[0] - vertexes[1]),
                                      vabs(vertexes[1] - vertexes[2]),
                                      vabs(vertexes[2] - vertexes[0])
                                  }};
    int width = std::max_element(sizes.begin(), sizes.end(), [](const Vec2i& l, const Vec2i& r) {return l.x < r.x;})->x;
    int height = std::max_element(sizes.begin(), sizes.end(), [](const Vec2i& l, const Vec2i& r) {return l.y < r.y;})->y;
    int anchor_x = std::min_element(vertexes.begin(), vertexes.end(), [](const Vec2i& l, const Vec2i& r) {return l.x < r.x;})->x;
    int anchor_y = std::min_element(vertexes.begin(), vertexes.end(), [](const Vec2i& l, const Vec2i& r) {return l.y < r.y;})->y;

    for(int y = anchor_y; y < anchor_y + height; ++y)
    {
        for(int x = anchor_x; x < anchor_x + width; ++x)
        {
            if(in_convex_hull(Vec2i(x,y), vertexes.begin(), vertexes.end()))
            {
                image.at(x,y) = color;
            }
        }
    }
}

} // end of namespace render

#endif // TRIANGLE_HPP
