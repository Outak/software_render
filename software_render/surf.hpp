#ifndef SURF_HPP
#define SURF_HPP

#include <algorithm>

#include "geometry/geometry.hpp"
#include "model/model.hpp"
#include "sdl/sdl.hpp"

#include "triangle.hpp"

namespace render
{
    inline void surf(model& m, sdl_texture& image, sdl_surface& screen_surface, Vec3f light_dir)
    {
        for (auto& face: m.faces)
        {
            std::array<Vec2i,3> screen_coords;
            std::array<Vec3f,3> world_coords;
            for (int j=0; j<3; j++)
            {
                Vec3f v = m.vertexes[face.coords[j]];
                screen_coords[j] = Vec2i((v.x+1.)*image.width()/2., (v.y+1.)*image.height()/2.);
                world_coords[j]  = v;
            }
            Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
            n.normalize();
            float intensity = n*light_dir;
            if (intensity>0) {
                triangle(screen_coords, image, screen_surface.map_rgb(intensity*255, intensity*255, intensity*255));
            }
        }
    }
}

#endif // SURF_HPP
