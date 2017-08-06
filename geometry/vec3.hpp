#ifndef VEC3_HPP
#define VEC3_HPP

#include <stdexcept>
#include <cmath>
#include <cassert>
#include <array>

#include <algorithm>
#include <numeric>

#include <ostream>

namespace cmn
{

template<class T>
class vec3
{
public:
    static const size_t dims = 3;
    static const size_t x_idx = 0;
    static const size_t y_idx = 1;
    static const size_t z_idx = 2;

    typedef std::array<T, dims> store_type;
    typedef typename store_type::value_type value_type;
    typedef typename store_type::reference reference;
    typedef typename store_type::const_reference const_reference;
    typedef typename store_type::pointer pointer;
    typedef typename store_type::const_pointer const_pointer;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::difference_type difference_type;
    typedef typename store_type::size_type size_type;

    vec3()
    {
        for(auto& val: _data)
        {
            val = (value_type)(0);
        }
    }

    vec3(const value_type& x, const value_type& y, const value_type& z)
    {
       (*this).x() = x;
       (*this).y() = y;
       (*this).z() = z;
    }

    vec3(const vec3& that) = default;
    vec3& operator=(const vec3& that) = default;

    vec3(vec3&& that) = default;
    vec3& operator=(vec3&& that) = default;

    template<class that_value_type>
    vec3(const vec3<that_value_type>& that)
    {
        std::copy(that.begin(), that.end(), begin());
    }

    template<class that_value_type>
    vec3<value_type>& operator=(const vec3<that_value_type>& that)
    {
        std::copy(that.begin(), that.end(), begin());
        return *this;
    }
    template<class that_value_type>
    vec3( vec3<that_value_type>&& that)
    {
        std::copy(that.begin(), that.end(), begin());
    }

    template<class that_value_type>
    vec3<value_type>& operator=(vec3<that_value_type>&& that)
    {
        std::copy(that.begin(), that.end(), begin());
        return *this;
    }

    vec3(const std::initializer_list<value_type>& init)
    {
        if (init.size() != _data.size())
        {
            throw std::length_error("vec3 initializer list size mismatch.");
        }
        std::copy(init.begin(), init.end(), begin());
    }

    vec3<value_type>& operator=(const std::initializer_list<value_type>& init)
    {
        if (init.size() != _data.size())
        {
            throw std::length_error("vec3 initializer list size mismatch.");
        }
        std::copy(init.begin(), init.end(), begin());
        return *this;
    }

    template<class that_value_type>
    vec3(const std::initializer_list<that_value_type>& init)
    {
        if (init.size() != _data.size())
        {
            throw std::length_error("vec3 initializer list size mismatch.");
        }
        std::copy(init.begin(), init.end(), begin());
    }

    template<class that_value_type>
    vec3<value_type>& operator=(const std::initializer_list<that_value_type>& init)
    {
        if (init.size() != _data.size())
        {
            throw std::length_error("vec3 initializer list size mismatch.");
        }
        std::copy(init.begin(), init.end(), begin());
        return *this;
    }

    ~vec3() {}

    reference& operator[](const size_type& idx)
    {
        if (idx >= size()) throw std::out_of_range("vec3 out of range error");
        return _data[idx];
    }

    const_reference& operator[](const size_type& idx) const
    {
        if (idx >= size()) throw std::out_of_range("vec3 out of range error");
        return _data[idx];
    }

    size_type size() const
    {
        return _data.size();
    }

    size_type max_size() const
    {
        return _data.max_size();
    }

    bool operator == (const vec3<value_type>& that) const
    {
        auto res = std::mismatch((*this).begin(), (*this).end(), that.begin());
        return (res.first == end() && res.second == that.end());
    }

    bool operator != (const vec3<value_type>& that) const
    {
        return !(*this == that);
    }

    bool empty() const
    {
        return _data.empty();
    }

    void clear()
    {
        for(auto& val: _data)
        {
            val = (value_type)(0);
        }
    }

    void swap(vec3<value_type>& that)
    {
        _data.swap(that._data);
    }

    vec3<value_type> operator - () const
    {
        vec3<value_type> tmp;
        std::transform((*this).begin(), (*this).end(), tmp.begin(), std::negate<value_type>());
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vec3<return_value_type> operator + (const vec3<that_value_type>& that) const
    {
        vec3<return_value_type> tmp;
        std::transform((*this).begin(), (*this).end(), that.begin(), tmp.begin(),
                     [](const return_value_type& f, const return_value_type& s) -> return_value_type
        {
            return f + s;
        });
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vec3<return_value_type> operator - (const vec3<that_value_type>& that) const
    {
        vec3<return_value_type> tmp;
        std::transform((*this).begin(), (*this).end(), that.begin(), tmp.begin(),
                     [](const return_value_type& f, const return_value_type& s) -> return_value_type
        {
            return f - s;
        });
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vec3<return_value_type>& operator += (const vec3<that_value_type>& that)
    {
        std::transform((*this).begin(), (*this).end(), that.begin(), (*this).begin(),
                     [](const return_value_type& f, const return_value_type& s) -> return_value_type
        {
            return f + (value_type)s;
        });
        return *this;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vec3<return_value_type>& operator -= (const vec3<that_value_type>& that)
    {
        std::transform((*this).begin(), (*this).end(), that.begin(), (*this).begin(),
                     [](const return_value_type& f, const return_value_type& s) -> return_value_type
        {
            return f - (value_type)s;
        });
        return *this;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vec3<return_value_type> operator + (const that_value_type& value) const
    {
        vec3<value_type> tmp;
        std::transform((*this).begin(), (*this).end(), tmp.begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val + (return_value_type)value;
        });
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vec3<return_value_type> operator - (const that_value_type& value) const
    {
        vec3<value_type> tmp;
        std::transform((*this).begin(), (*this).end(), tmp.begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val - (return_value_type)value;
        });
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vec3<return_value_type> operator += (const that_value_type& value)
    {
        std::transform((*this).begin(), (*this).end(), (*this).begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val + (return_value_type)value;
        });
        return *this;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vec3<return_value_type> operator -= (const that_value_type& value)
    {
        std::transform((*this).begin(), (*this).end(), (*this).begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val - (return_value_type)value;
        });
        return *this;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vec3<return_value_type> operator * (const that_value_type& value) const
    {
        vec3<value_type> tmp;
        std::transform((*this).begin(), (*this).end(), tmp.begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val * (return_value_type)value;
        });
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vec3<return_value_type> operator / (const that_value_type& value) const
    {
        vec3<value_type> tmp;
        std::transform((*this).begin(), (*this).end(), tmp.begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val / (return_value_type)value;
        });
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vec3<return_value_type> operator *= (const that_value_type& value)
    {
        std::transform((*this).begin(), (*this).end(), (*this).begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val * (return_value_type)value;
        });
        return *this;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vec3<return_value_type> operator /= (const that_value_type& value)
    {
        std::transform((*this).begin(), (*this).end(), (*this).begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val / (return_value_type)value;
        });
        return *this;
    }

    template<class that_value_type>
    double operator * (const vec3<that_value_type>& that) const
    {
        return (*this).prod(that);
    }

    double prod(const vec3<value_type>& that) const
    {
        return  std::inner_product((*this).begin(), (*this).end(), that.begin(), 0.0);
    }

    template<class that_value_type>
    double prod(const vec3<that_value_type>& that) const
    {
        return  std::inner_product((*this).begin(), (*this).end(), that.begin(), 0.0);
    }

    double norm() const
    {
        return std::sqrt((*this).prod(*this));
    }

    vec3<value_type> abs() const
    {
        vec3<value_type> tmp;
        std::transform((*this).begin(), (*this).end(), tmp.begin(), [](const_reference val){return std::abs(val);});
        return tmp;
    }

    cmn::vec3<value_type> vec_prod(const cmn::vec3<value_type> &v) const
    {
        return cmn::vec3<value_type>(y()*v.z()-z()*v.y()
                                     , z()*v.x()-x()*v.z()
                                     , x()*v.y()-y()*v.x());
    }

    cmn::vec3<value_type> normalize(const_reference l = (value_type)1) const
    {
        return (*this)*(l/norm());
    }

    iterator begin()
    {
        return _data.begin();
    }

    iterator end()
    {
        return _data.end();
    }

    const_iterator begin() const
    {
        return _data.cbegin();
    }

    const_iterator end() const
    {
        return _data.cend();
    }

    const_iterator cbegin()
    {
        return _data.cbegin();
    }

    const_iterator cend()
    {
        return _data.cend();
    }

    reference x()
    {
        return _data[x_idx];
    }

    const_reference x() const
    {
        return _data[x_idx];
    }

    reference y()
    {
        return _data[y_idx];
    }

    const_reference y() const
    {
        return _data[y_idx];
    }

    reference z()
    {
        return _data[z_idx];
    }

    const_reference z() const
    {
        return _data[z_idx];
    }

private:
    store_type _data;
};

typedef vec3<int> vec3i;
typedef vec3<double> vec3f;

} // end of cmn namespace

template<class value_type>
std::ostream& operator << (std::ostream& out, const cmn::vec3<value_type>& vector)
{
    auto it = vector.begin();
    out << '[' << *(it++);
    for(;it != vector.end(); ++it)
    {
        out << ", " << *it;
    }
    out << ']';
    return out;
}

template<class value_type, class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type
         , class = typename std::enable_if<std::is_arithmetic<that_value_type>::value>::type>
cmn::vec3<return_value_type> operator + (const that_value_type& val, const cmn::vec3<value_type>& v)
{
    return (v + val);
}

template<class value_type, class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type
         , class = typename std::enable_if<std::is_arithmetic<that_value_type>::value>::type>
cmn::vec3<return_value_type> operator - (const that_value_type& val, const cmn::vec3<value_type>& v)
{
    return -(v - val);
}

template<class value_type, class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type
         , class = typename std::enable_if<std::is_arithmetic<that_value_type>::value>::type>
cmn::vec3<return_value_type> operator * (const that_value_type& val, const cmn::vec3<value_type>& v)
{
    return (v * val);
}

#endif // VEC3_HPP
