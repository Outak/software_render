#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <array>

#include "vec2.hpp"
#include "vec3.hpp"

typedef cmn::vec2f point2d;
typedef cmn::vec3f point3d;

typedef std::array<point2d, 2> line2d;
typedef std::array<point3d, 2> line3d;

typedef std::array<point2d, 3> triangle2d;
typedef std::array<point3d, 3> triangle3d;

typedef std::array<point2d, 4> rect2d;
typedef std::array<point3d, 4> rect3d;

#endif // PRIMITIVES_HPP
