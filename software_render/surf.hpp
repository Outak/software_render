#ifndef SURF_HPP
#define SURF_HPP

#include <algorithm>

#include "geometry/geometry.hpp"
#include "model/model.hpp"
#include "sdl/sdl.hpp"

#include "triangle.hpp"

namespace render
{
    inline void surf(model& m, sdl_texture& image, sdl_surface& screen_surface, cmn::vec3f light_dir)
    {
        for (auto& face: m.faces)
        {
            triangle2d screen_coords;
            triangle3d world_coords;
            for (int j=0; j<3; j++)
            {
                point3d &v = m.vertexes[face.coords[j]];
                screen_coords[j] = point2d((v.x() + 1.)*image.width()/2. , (v.y() + 1.)*image.height()/2.);
                world_coords[j]  = v;
            }
            point3d n = (world_coords[2]-world_coords[0]).vec_prod(world_coords[1]-world_coords[0]);
            n = n.normalize();
            float intensity = n*light_dir;
            if (intensity>0) {
                triangle(screen_coords, image, screen_surface.map_rgb(intensity*255, intensity*255, intensity*255));
            }
        }
    }
}

#endif // SURF_HPP
