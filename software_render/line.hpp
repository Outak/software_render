#ifndef LINE_HPP
#define LINE_HPP

#include "geometry/geometry.hpp"
#include "sdl/sdl.hpp"

namespace render
{

inline void line(int x0, int y0, int x1, int y1, sdl_texture& image, const uint32_t& color)
{ // Bresenham's line algorithm
    bool steep = false;
    if (std::abs(x0-x1) < std::abs(y0-y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    for (int x=x0, y = y0, y_error = 0, dy_error = 2*std::abs(y1 - y0), dx_error = (x1-x0); x<=x1; x++, y_error += dy_error)
    {
        if (steep) {
            image.at(y, x) = color; // if transposed, de-transpose
        } else {
            image.at(x, y) = color;
        }
        if (y_error > dx_error)
        {
             y += (y1 > y0) ? 1 : -1;
             y_error -= 2*dx_error;
        }
    }
}

inline void line(const Vec2i& st, const Vec2i& fn, sdl_texture& image, const uint32_t& color)
{
    line(st.x, st.y, fn.x, fn.y, image, color);
}

inline void line(const line2d& ln, sdl_texture& image, const uint32_t& color)
{
    line(ln[0], ln[1], image, color);
}

} // end of namespace render

#endif // LINE_HPP
