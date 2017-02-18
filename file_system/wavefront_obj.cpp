#include "wavefront_obj.hpp"

#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>

model wavefront_obj::read_model(std::istream& input)
{
    if (!input.good())
    {
        throw std::runtime_error("can't open wavefront.obj file");
    }
    model new_model;
    std::string line;
    while (!input.eof()) {
        std::getline(input, line);
        switch (parse_line_type(line)) {
        case line_type::vertex:
            new_model.vertexes.push_back(read_vertex(line));
            break;
        case line_type::texture:
            new_model.texture_vertexes.push_back(read_texture_coords(line));
            break;
        case line_type::normal:
            new_model.normals.push_back(read_normal(line));
            break;
        case line_type::face:
            new_model.faces.push_back(read_face(line));
            break;
        default:
            // throw std::runtime_error("wavefront.obj unexpected line type"); // ignore vp lines
            break;
        }
    }
    return new_model;
}

wavefront_obj::line_type wavefront_obj::parse_line_type(std::string& input)
{
    std::istringstream iss(input);
    std::string line_caption;
    iss >> line_caption;
    line_type type = line_type::none;
    if (line_caption == "v") {
        type = line_type::vertex;
    } else if (line_caption == "vt") {
        type = line_type::texture;
    } else if (line_caption == "vn") {
        type = line_type::normal;
    } else if (line_caption == "f") {
        type = line_type::face;
    }
    std::getline(iss, input); // remove caption
    return type;
}

Vec3f wavefront_obj::read_vertex(const std::string& input)
{
    std::istringstream iss(input);
    Vec3f tmp;
    iss >> tmp.x;
    iss >> tmp.y;
    iss >> tmp.z;
    return tmp;
}

Vec3f wavefront_obj::read_texture_coords(const std::string& input)
{
    std::istringstream iss(input);
    Vec3f tmp;
    iss >> tmp.x;
    iss >> tmp.y;
    iss >> tmp.z;
    return tmp;
}

Vec3f wavefront_obj::read_normal(const std::string& input)
{
    std::istringstream iss(input);
    Vec3f tmp;
    iss >> tmp.x;
    iss >> tmp.y;
    iss >> tmp.z;
    return tmp;
}

model::face_t wavefront_obj::read_face(const std::string& input)
{
    std::istringstream iss(input);
    char divider = '/';
    model::face_t tmp;
    for(int i = 0; i < 3; ++i)
    {
        iss >> tmp.coords[i] >> divider >> tmp.texture[i] >> divider >> tmp.normals[i];
        --tmp.coords[i]; --tmp.texture[i]; --tmp.normals[i];
    }
    return tmp;
}
