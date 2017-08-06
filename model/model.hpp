#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>

#include "geometry/geometry.hpp"

class model
{
public:
    class face_t
    {
    public:
        point3d coords;
        point3d texture;
        point3d normals;
    };

    model();

    std::vector<point3d> vertexes;
    std::vector<point3d> texture_vertexes;
    std::vector<point3d> normals;
    std::vector<face_t> faces;
};

#endif // MODEL_HPP
