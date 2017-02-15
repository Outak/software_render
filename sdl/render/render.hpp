#ifndef RENDER_HPP
#define RENDER_HPP

#include <memory>

#include "sdl/window/window.hpp"
#include "sdl/surface/surface.hpp"

struct SDL_Renderer;

class sdl_render
{
    class render_deleter
    {
    public:
        void operator()(SDL_Renderer* render);
    };

public:
    sdl_render(sdl_window& window);
    sdl_render(sdl_surface& surface);

    SDL_Renderer* get();
    const SDL_Renderer* get() const;

private:
    std::unique_ptr<SDL_Renderer, render_deleter> _render;
};

#endif // RENDER_HPP
