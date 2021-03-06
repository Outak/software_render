#ifndef COMMON_HPP
#define COMMON_HPP

#include <algorithm>

#include "geometry/geometry.hpp"

namespace render
{

inline bool clockwise_less(const cmn::vec2i& a, const cmn::vec2i& b, const cmn::vec2i& center)
{
    if (a.x() - center.x() >= 0 && b.x() - center.x() < 0)
        return true;
    if (a.x() - center.x() < 0 && b.x() - center.x() >= 0)
        return false;
    if (a.x() - center.x() == 0 && b.x() - center.x() == 0) {
        if (a.y() - center.y() >= 0 || b.y() - center.y() >= 0)
            return a.y() > b.y();
        return b.y() > a.y();
    }

    // compute the cross product of vectors (center -> a) x (center -> b)
    int det = (a.x() - center.x()) * (b.y() - center.y()) - (b.x() - center.x()) * (a.y() - center.y());
    if (det < 0)
        return true;
    if (det > 0)
        return false;

    // points a and b are on the same line from the center
    // check which point is closer to the center
    int d1 = (a.x() - center.x()) * (a.x() - center.x()) + (a.y() - center.y()) * (a.y() - center.y());
    int d2 = (b.x() - center.x()) * (b.x() - center.x()) + (b.y() - center.y()) * (b.y() - center.y());
    return d1 > d2;
}

inline bool clockwise_side(const cmn::vec2i& p, const cmn::vec2i& lst, const cmn::vec2i& lfn)
{
    cmn::vec2i normal(lfn.y() - lst.y(), lst.x() - lfn.x());
    return ((p - lst)*normal) >= 0.0;
}

template<class hull_it>
inline bool in_convex_hull(const cmn::vec2i& p, hull_it begin, hull_it end)
{
    for(auto edge = (begin + 1); edge != end; ++edge)
    {
        if (!clockwise_side(p, *(edge - 1), *edge))
        {
            return false;
        }
    }
    return clockwise_side(p, *(end-1), *begin);
}

} // end of namespace render

#endif // COMMON_HPP
