#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <memory>

#include "sdl/render/render.hpp"
#include "sdl/surface/surface.hpp"

#include <SDL2/SDL_blendmode.h>

struct SDL_Texture;

class sdl_texture
{
    class texture_deleter
    {
    public:
        void operator()(SDL_Texture* texture);
    };

public:
    sdl_texture(sdl_render& render, sdl_surface& surf);

    SDL_Texture* get();

    const SDL_Texture* get() const;

    void set_color(uint8_t red, uint8_t green, uint8_t blue);

    void set_blend_mode(SDL_BlendMode blending);

    void set_alpha(uint8_t alpha);

    int width();

    int height();

    //Pixel manipulators
    void lockTexture();

    void unlockTexture();

    void* pixels();

    int pitch();

    uint32_t& at(int x, int y);

    void render();

    void render(int x, int y, int width, int height);

private:
    sdl_render& _render;
    std::unique_ptr<SDL_Texture, texture_deleter> _texture;
    void* _pixels;
    int _pitch;
    int _width;
    int _height;
};

#endif // TEXTURE_HPP
