#include "surface.hpp"

#include <SDL2/SDL_surface.h>

void sdl_surface_store::surface_deleter::operator()(SDL_Surface* surface)
{
    SDL_FreeSurface(surface);
}

sdl_surface_store::sdl_surface_store(SDL_Surface* surface0) :
    _surface(surface0)
{
}

SDL_Surface*sdl_surface_store::get()
{
    return _surface.get();
}

int sdl_surface_store::width()
{
    return _surface.get()->w;
}

int sdl_surface_store::height()
{
    return _surface.get()->h;
}

uint32_t sdl_surface_store::foramt()
{
    return _surface.get()->format->format;
}

SDL_PixelFormat* sdl_surface_store::pix_foramt()
{
    return _surface.get()->format;
}

void sdl_surface_view::empty_deleter::operator()(SDL_Surface* surface)
{}

sdl_surface_view::sdl_surface_view(SDL_Surface* surface) :
    _surface(surface)
{
}

SDL_Surface*sdl_surface_view::get()
{
    return _surface.get();
}

int sdl_surface_view::width()
{
    return _surface.get()->w;
}

int sdl_surface_view::height()
{
    return _surface.get()->h;
}

uint32_t sdl_surface_view::foramt()
{
    return _surface.get()->format->format;
}

SDL_PixelFormat* sdl_surface_view::pix_foramt()
{
    return _surface.get()->format;
}
