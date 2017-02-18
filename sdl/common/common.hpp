#ifndef SDL_COMMON_HPP
#define SDL_COMMON_HPP

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

#endif // SDL_COMMON_HPP
