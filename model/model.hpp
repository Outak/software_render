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
        std::array<int, 3> coords;
        std::array<int, 3> texture;
        std::array<int, 3> normals;
    };

    model();

    std::vector<Vec3f> vertexes;
    std::vector<Vec3f> texture_vertexes;
    std::vector<Vec3f> normals;
    std::vector<face_t> faces;
};

#endif // MODEL_HPP
