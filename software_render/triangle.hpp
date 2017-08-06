#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <algorithm>

#include "geometry/geometry.hpp"
#include "common.hpp"

#include "zbuffer.hpp"

namespace render
{

//inline bool in_triangle (const double &detT, const point2d &lambda0_k0, const point2d &lambda1_k0, const triangle2d &vertexes, const point2d& p)
//{
//    const point2d tmp = p - vertexes[2];
//    const double lambda0 = lambda0_k0*tmp;
//    const double lambda1 = lambda1_k0*tmp;
//    const double lambda2 = detT - lambda0 - lambda1;
//    return (lambda0*detT >= 0.0) && (lambda1*detT >= 0.0) && (lambda2*detT >= 0.0);
//}

inline void triangle(const triangle2d &vertexes, sdl_texture &image, const uint32_t &color)
{
    std::array<point2d, 3> sizes = {{
                                          (vertexes[0] - vertexes[1]).abs(),
                                          (vertexes[1] - vertexes[2]).abs(),
                                          (vertexes[2] - vertexes[0]).abs()
                                      }};
    float width = std::max_element(sizes.begin(), sizes.end(), [](const point2d& l, const point2d& r) {return l.x() < r.x();})->x();
    float height = std::max_element(sizes.begin(), sizes.end(), [](const point2d& l, const point2d& r) {return l.y() < r.y();})->y();
    float anchor_x = std::min_element(vertexes.begin(), vertexes.end(), [](const point2d& l, const point2d& r) {return l.x() < r.x();})->x();
    float anchor_y = std::min_element(vertexes.begin(), vertexes.end(), [](const point2d& l, const point2d& r) {return l.y() < r.y();})->y();

    const double detT = (vertexes[0].x() - vertexes[2].x())*(vertexes[1].y() - vertexes[2].y())
                    - (vertexes[0].y() - vertexes[2].y())*(vertexes[1].x() - vertexes[2].x());
    const point2d lambda0_k0((vertexes[1].y() - vertexes[2].y()), (vertexes[2].x() - vertexes[1].x()));
    const point2d lambda1_k0((vertexes[2].y() - vertexes[0].y()), (vertexes[0].x() - vertexes[2].x()));

    const auto in_triangle = [detT, lambda0_k0, lambda1_k0](const point2d& p) -> bool
    {
        const double lambda0 = lambda0_k0*p;
        const double lambda1 = lambda1_k0*p;
        const double lambda2 = detT - lambda0 - lambda1;
        return (lambda0*detT >= 0.0) && (lambda1*detT >= 0.0) && (lambda2*detT >= 0.0);
    };

    for(float y = anchor_y; y < anchor_y + height; ++y)
    {
        for(float x = anchor_x; x < anchor_x + width; ++x)
        {
            if (in_triangle(point2d(x,y) - vertexes[2]))
            //if (in_triangle(detT, lambda0_k0, lambda1_k0, vertexes, point2d(x,y)))
            {
                image.at(x,y) = color;
            }
        }
    }
}

inline void triangle_3d(const triangle3d &vertexes, sdl_texture& image, const uint32_t& color, z_buffer& zbuffer)
{
}

} // end of namespace render

#endif // TRIANGLE_HPP
