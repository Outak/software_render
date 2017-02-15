#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdexcept>
#include <string>

#include <SDL2/SDL_error.h>

class sdl_exception : std::exception
{
public:
    sdl_exception(const std::string& msg);

private:
    std::string _msg;
};

#endif // COMMON_HPP
