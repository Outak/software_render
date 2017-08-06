#ifndef SIZE2_H
#define SIZE2_H

#include <ostream>
#include <utility>

#include "vec2.hpp"

namespace cmn {

template<class T>
class size
{
public:
    typedef T value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    size() : _width(0), _height(0) {}

    size(const size& that) = default;
    size(size&& that) = default;

    size& operator=(const size& that) = default;
    size& operator=(size&& that) = default;

    size(const_reference width0, const_reference height0) : _width(width0), _height(height0)
    {}

    template<class that_value_type>
    size(const vec2<that_value_type> &v) : _width(v.x()), _height(v.y())
    {}

    template<class that_value_type>
    size(const that_value_type& width0, const that_value_type& height0) :
        _width(static_cast<value_type>(width0)), _height(static_cast<value_type>(height0))
    {}

    template<class that_value_type>
    size<value_type>& operator = (const size<that_value_type>& size_other)
    {
        _width = static_cast<value_type>(size_other._width);
        _height = static_cast<value_type>(size_other._height);
        return *this;
    }

    bool empty() const
    {
        return (_width == (value_type)0) && (_height == (value_type)0);
    }

    template<class that_value_type>
    bool operator == (const size<that_value_type>& s) const
    {
        return (_width == s._width) && (_height == s._height);
    }

    template<class that_value_type>
    bool operator != (const size<that_value_type>& that) const
    {
        return !(*this == that);
    }

    void swap(size<value_type>& that)
    {
        std::swap(this->_width, that._width);
        std::swap(this->_height, that._height);
    }

    value_type sqr() const
    {
        return _width*_height;
    }

    template<class that_value_type>
    size<value_type> operator+(const size<that_value_type>& s) const
    {
        return size<value_type>(_width + s._width, _height + s._height);
    }

    template<class that_value_type>
    size<value_type> operator-(const size<that_value_type>& s) const
    {
        return size<value_type>(_width - s._width, _height - s._height);
    }

    template<class that_value_type>
    size<value_type>& operator+=(const size<that_value_type>& s)
    {
        *this = *this + s;
        return *this;
    }

    template<class that_value_type>
    size<value_type>& operator-=(const size<that_value_type>& s)
    {
        *this = *this - s;
        return *this;
    }

    template<class that_value_type>
    size<value_type> operator*(const that_value_type& val) const
    {
        return size<value_type>(_width*val, _height*val);
    }

    template<class that_value_type>
    size<value_type> operator/(const that_value_type& val) const
    {
        return size<value_type>(_width/val, _height/val);
    }

    template<class that_value_type>
    size<value_type>& operator*=(const that_value_type& val)
    {
        *this = *this * val;
        return *this;
    }

    template<class that_value_type>
    size<value_type>& operator/=(const that_value_type& val)
    {
        *this = *this / val;
        return *this;
    }

    reference width()
    {
        return _width;
    }

    const_reference width() const
    {
        return _width;
    }

    reference height()
    {
        return _height;
    }

    const_reference height() const
    {
        return _height;
    }

private:
    value_type _width;
    value_type _height;
};

typedef cmn::size<int> size2i;
typedef cmn::size<double> size2f;

}  // end of cmn namespace

template<class value_t>
std::ostream& operator << (std::ostream& out, const cmn::size<value_t>& val)
{
    out << "S(" << val.width() << ", " << val.height() << ')';
    return out;
}

template<class size_type, class value_type>
cmn::size<size_type> operator*(const value_type& val, const cmn::size<size_type>& s)
{
    return s * val;
}

#endif // SIZE2_H
