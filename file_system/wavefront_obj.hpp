#ifndef WAVEFRONT_OBJ
#define WAVEFRONT_OBJ

#include <istream>

#include "model/model.hpp"

class wavefront_obj
{
public:
    enum class line_type
    {
        none
        ,coment
        ,empty
        ,vertex
        ,texture
        ,normal
        ,face
    };

    wavefront_obj() {}

    static model read_model(std::istream& input);

    static line_type parse_line_type(std::string& input);
    static Vec3f read_vertex(const std::string& input);
    static Vec3f read_texture_coords(const std::string& input);
    static Vec3f read_normal(const std::string& input);
    static model::face_t read_face(const std::string& input);
};

#endif // WAVEFRONT_OBJ
