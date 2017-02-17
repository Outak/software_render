#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <array>

#include "vec2.hpp"
#include "vec3.hpp"

typedef Vec2i point2d;
typedef Vec3i point3d;

typedef std::array<Vec2i, 2> line2d;
typedef std::array<Vec3i, 2> line3d;

typedef std::array<Vec2i, 3> triangle2d;
typedef std::array<Vec3i, 3> triangle3d;

typedef std::array<Vec2i, 4> rect2d;
typedef std::array<Vec3i, 4> rect3d;

#endif // PRIMITIVES_HPP
