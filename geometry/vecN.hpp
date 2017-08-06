#ifndef VECN_HPP
#define VECN_HPP

#include <stdexcept>
#include <cmath>
#include <cassert>
#include <array>

#include <algorithm>
#include <numeric>

#include <ostream>

namespace cmn
{

template<class T, size_t N>
class vecn
{
public:
    static const size_t dims = N;

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

    vecn()
    {
        for(auto& val: _data)
        {
            val = (value_type)(0);
        }
    }

    vecn(const vecn& that) = default;
    vecn& operator=(const vecn& that) = default;

    vecn(vecn&& that) = default;
    vecn& operator=(vecn&& that) = default;

    template<class that_value_type>
    vecn(const vecn<that_value_type, dims>& that)
    {
        std::copy(that.begin(), that.end(), begin());
    }

    template<class that_value_type>
    vecn<value_type, dims>& operator=(const vecn<that_value_type, dims>& that)
    {
        std::copy(that.begin(), that.end(), begin());
        return *this;
    }
    template<class that_value_type>
    vecn( vecn<that_value_type, dims>&& that)
    {
        std::copy(that.begin(), that.end(), begin());
    }

    template<class that_value_type>
    vecn<value_type, dims>& operator=(vecn<that_value_type, dims>&& that)
    {
        std::copy(that.begin(), that.end(), begin());
        return *this;
    }

    vecn(const std::initializer_list<value_type>& init)
    {
        if (init.size() != _data.size())
        {
            throw std::length_error("vecn initializer list size mismatch.");
        }
        std::copy(init.begin(), init.end(), begin());
    }

    vecn<value_type, dims>& operator=(const std::initializer_list<value_type>& init)
    {
        if (init.size() != _data.size())
        {
            throw std::length_error("vecn initializer list size mismatch.");
        }
        std::copy(init.begin(), init.end(), begin());
        return *this;
    }

    template<class that_value_type>
    vecn(const std::initializer_list<that_value_type>& init)
    {
        if (init.size() != _data.size())
        {
            throw std::length_error("vecn initializer list size mismatch.");
        }
        std::copy(init.begin(), init.end(), begin());
    }

    template<class that_value_type>
    vecn<value_type, dims>& operator=(const std::initializer_list<that_value_type>& init)
    {
        if (init.size() != _data.size())
        {
            throw std::length_error("vecn initializer list size mismatch.");
        }
        std::copy(init.begin(), init.end(), begin());
        return *this;
    }

    ~vecn() {}

    reference& operator[](const size_type& idx)
    {
        if (idx >= size()) throw std::out_of_range("vecn out of range error");
        return _data[idx];
    }

    const_reference& operator[](const size_type& idx) const
    {
        if (idx >= size()) throw std::out_of_range("vecn out of range error");
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

    bool operator == (const vecn<value_type, dims>& that) const
    {
        auto res = std::mismatch((*this).begin(), (*this).end(), that.begin());
        return (res.first == end() && res.second == that.end());
    }

    bool operator != (const vecn<value_type, dims>& that) const
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

    void swap(vecn<value_type, dims>& that)
    {
        _data.swap(that._data);
    }

    vecn<value_type, dims> operator - () const
    {
        vecn<value_type, dims> tmp;
        std::transform((*this).begin(), (*this).end(), tmp.begin(), std::negate<value_type>());
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vecn<return_value_type, dims> operator + (const vecn<that_value_type, dims>& that) const
    {
        vecn<return_value_type, dims> tmp;
        std::transform((*this).begin(), (*this).end(), that.begin(), tmp.begin(),
                     [](const return_value_type& f, const return_value_type& s) -> return_value_type
        {
            return f + s;
        });
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vecn<return_value_type, dims> operator - (const vecn<that_value_type, dims>& that) const
    {
        vecn<return_value_type, dims> tmp;
        std::transform((*this).begin(), (*this).end(), that.begin(), tmp.begin(),
                     [](const return_value_type& f, const return_value_type& s) -> return_value_type
        {
            return f - s;
        });
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vecn<return_value_type, dims>& operator += (const vecn<that_value_type, dims>& that)
    {
        std::transform((*this).begin(), (*this).end(), that.begin(), (*this).begin(),
                     [](const return_value_type& f, const return_value_type& s) -> return_value_type
        {
            return f + (value_type)s;
        });
        return *this;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vecn<return_value_type, dims>& operator -= (const vecn<that_value_type, dims>& that)
    {
        std::transform((*this).begin(), (*this).end(), that.begin(), (*this).begin(),
                     [](const return_value_type& f, const return_value_type& s) -> return_value_type
        {
            return f - (value_type)s;
        });
        return *this;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vecn<return_value_type, dims> operator + (const that_value_type& value) const
    {
        vecn<value_type, dims> tmp;
        std::transform((*this).begin(), (*this).end(), tmp.begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val + (return_value_type)value;
        });
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vecn<return_value_type, dims> operator - (const that_value_type& value) const
    {
        vecn<value_type, dims> tmp;
        std::transform((*this).begin(), (*this).end(), tmp.begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val - (return_value_type)value;
        });
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vecn<return_value_type, dims> operator += (const that_value_type& value)
    {
        std::transform((*this).begin(), (*this).end(), (*this).begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val + (return_value_type)value;
        });
        return *this;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vecn<return_value_type, dims> operator -= (const that_value_type& value)
    {
        std::transform((*this).begin(), (*this).end(), (*this).begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val - (return_value_type)value;
        });
        return *this;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vecn<return_value_type, dims> operator * (const that_value_type& value) const
    {
        vecn<value_type, dims> tmp;
        std::transform((*this).begin(), (*this).end(), tmp.begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val * (return_value_type)value;
        });
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vecn<return_value_type, dims> operator / (const that_value_type& value) const
    {
        vecn<value_type, dims> tmp;
        std::transform((*this).begin(), (*this).end(), tmp.begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val / (return_value_type)value;
        });
        return tmp;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vecn<return_value_type, dims> operator *= (const that_value_type& value)
    {
        std::transform((*this).begin(), (*this).end(), (*this).begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val * (return_value_type)value;
        });
        return *this;
    }

    template<class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type>
    vecn<return_value_type, dims> operator /= (const that_value_type& value)
    {
        std::transform((*this).begin(), (*this).end(), (*this).begin(),
                     [&value](const return_value_type& val) -> return_value_type
        {
            return val / (return_value_type)value;
        });
        return *this;
    }

    template<class that_value_type>
    double operator * (const vecn<that_value_type, dims>& that) const
    {
        return (*this).prod(that);
    }

    double prod(const vecn<value_type, dims>& that) const
    {
        return  std::inner_product((*this).begin(), (*this).end(), that.begin(), 0.0);
    }

    template<class that_value_type>
    double prod(const vecn<that_value_type, dims>& that) const
    {
        return  std::inner_product((*this).begin(), (*this).end(), that.begin(), 0.0);
    }

    double norm() const
    {
        return std::sqrt((*this).prod(*this));
    }

    vecn<value_type, dims> abs() const
    {
        vecn<value_type, dims> tmp;
        std::transform((*this).begin(), (*this).end(), tmp.begin(), [](const_reference val){return std::abs(val);});
        return tmp;
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

private:
    store_type _data;
};

} // end of cmn namespace

template<class value_type, size_t dims>
std::ostream& operator << (std::ostream& out, const cmn::vecn<value_type, dims>& vector)
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

template<class value_type, size_t dims, class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type
         , class = typename std::enable_if<std::is_arithmetic<that_value_type>::value>::type>
cmn::vecn<return_value_type, dims> operator + (const that_value_type& val, const cmn::vecn<value_type, dims>& v)
{
    return (v + val);
}

template<class value_type, size_t dims, class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type
         , class = typename std::enable_if<std::is_arithmetic<that_value_type>::value>::type>
cmn::vecn<return_value_type, dims> operator - (const that_value_type& val, const cmn::vecn<value_type, dims>& v)
{
    return -(v - val);
}

template<class value_type, size_t dims, class that_value_type, class return_value_type = typename std::common_type<value_type, that_value_type>::type
         , class = typename std::enable_if<std::is_arithmetic<that_value_type>::value>::type>
cmn::vecn<return_value_type, dims> operator * (const that_value_type& val, const cmn::vecn<value_type, dims>& v)
{
    return (v * val);
}

#endif // VECN_HPP
