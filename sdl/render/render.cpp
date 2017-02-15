#include "render.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

void sdl_render::render_deleter::operator()(SDL_Renderer* render)
{
    SDL_DestroyRenderer(render);
}

sdl_render::sdl_render(sdl_window& window) :
    _render(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED))
{

}

sdl_render::sdl_render(sdl_surface& surface) :
    _render(SDL_CreateSoftwareRenderer(surface.get()))
{

}

SDL_Renderer*sdl_render::get()
{
    return _render.get();
}

const SDL_Renderer*sdl_render::get() const
{
    return _render.get();
}

