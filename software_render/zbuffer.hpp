#ifndef ZBUFFER_HPP
#define ZBUFFER_HPP

#include <memory>
#include <limits>

class z_buffer
{
public:

private:
    std::unique_ptr<int> _buffer;
    int _width;
    int _height;
};

#endif // ZBUFFER_HPP
