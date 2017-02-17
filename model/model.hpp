#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <sstream>

#include "geometry/geometry.hpp"

class model
{
public:
    typedef std::array<int, 3> face_t;

    model(const std::string& file_name);

    int nverts();

    int nfaces();

    face_t& face(int idx);

    Vec3f& vert(int i);

private:
    std::vector<Vec3f> _vertexes;
    std::vector<face_t> _faces;
};

#endif // MODEL_HPP
