#include "common.hpp"

sdl_exception::sdl_exception(const std::string& msg):
    _msg(msg + " SDL_Error : " + SDL_GetError())
{}
