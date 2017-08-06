#ifndef MEAH_HPP
#define MEAH_HPP

#include "geometry/geometry.hpp"
#include "model/model.hpp"
#include "sdl/sdl.hpp"

#include "line.hpp"

namespace render
{
    inline void mesh(model& m, sdl_texture& image, const uint32_t& color)
    {
        for (auto& face: m.faces)
        {
            for (int j=0; j<3; j++) {
                cmn::vec3f v0 = m.vertexes[face.coords[j]];
                cmn::vec3f v1 = m.vertexes[face.coords[(j+1)%3]];
                int x0 = (v0.x()+1.)*image.width()/2.;
                int y0 = (v0.y()+1.)*image.height()/2.;
                int x1 = (v1.x()+1.)*image.width()/2.;
                int y1 = (v1.y()+1.)*image.height()/2.;
                line(x0, y0, x1, y1, image, color);
            }
        }
    }
}

#endif // MEAH_HPP
