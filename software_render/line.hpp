#ifndef LINE_HPP
#define LINE_HPP

#include "geometry/geometry.hpp"
#include "sdl/sdl.hpp"

namespace render
{

inline void line_new(int x0, int y0, int x1, int y1, sdl_texture& image, const uint32_t& color)
{ // Bresenham's line algorithm
    int delta_max = std::max(std::abs(x0-x1),std::abs(y0-y1));

    for (int t = 0
             , x = x0
             , y = y0
             , x_error = 0
             , y_error = 0
             , dx_error = 2*std::abs(x1 - x0)
             , dy_error = 2*std::abs(y1 - y0)
             , dx = (x1 - x0 > 0)?1:-1
             , dy = (y1 - y0 > 0)?1:-1
             , delta_max_minus = -delta_max;

                t <= delta_max;

                    ++t
                  , x_error += dx_error
                  , y_error += dy_error)
    {
        image.at(x, y) = color;
        int m =  (((uint32_t)(x_error + delta_max_minus) & 0x80000000)>>31)^0x00000001;
        x += dx * m;
        x_error -= 2*m*delta_max;
        m =  (((uint32_t)(y_error + delta_max_minus) & 0x80000000)>>31)^0x00000001;
        y += dy * m;
        y_error -= 2*m*delta_max;
    }
}

inline void line(int x0, int y0, int x1, int y1, sdl_texture& image, const uint32_t& color)
{ // Bresenham's line algorithm
    bool transposed = false;
    if (std::abs(x0-x1) < std::abs(y0-y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        transposed = true;
    }
    if (x0>x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    for (int x = x0,
             y = y0,
             y_error = 0,
             dx_error = (x1 - x0),
             dy_error = 2*std::abs(y1 - y0),
             dy = (y1 > y0) ? 1 : -1;

                x<=x1;

                    ++x,
                    y_error += dy_error)
    {
        if (transposed) {
            image.at(y, x) = color;
        } else {
            image.at(x, y) = color;
        }
        if (y_error > dx_error)
        {
             y += dy;
             y_error -= 2*dx_error;
        }
    }
}

inline void line(const cmn::vec2i& st, const cmn::vec2i& fn, sdl_texture& image, const uint32_t& color)
{
    line(st.x(), st.y(), fn.x(), fn.y(), image, color);
}

inline void line(const line2d& ln, sdl_texture& image, const uint32_t& color)
{
    line(ln[0], ln[1], image, color);
}

} // end of namespace render

#endif // LINE_HPP
