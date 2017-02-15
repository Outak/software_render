#include "window.hpp"

#include "sdl/common/common.hpp"

#include <SDL2/SDL_video.h>

void sdl_window::widow_deleter::operator()(SDL_Window* window)
{
    SDL_DestroyWindow(window);
}

sdl_window::sdl_window(const std::string& name, const int& w, const int& h) :
    _window(SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN))
{
    if(!_window)
    {
        throw sdl_exception("Window could not be created!");
    }
}

SDL_Window*sdl_window::get()
{
    return _window.get();
}

const SDL_Window*sdl_window::get() const
{
    return _window.get();
}

sdl_surface_view sdl_window::surface()
{
    return sdl_surface_view(SDL_GetWindowSurface(_window.get()));
}

void sdl_window::update()
{
    SDL_UpdateWindowSurface(_window.get());
}
