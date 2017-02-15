#include "texture.hpp"

#include "sdl/common/common.hpp"

#include <SDL2/SDL_render.h>

void sdl_texture::texture_deleter::operator()(SDL_Texture* texture)
{
    SDL_DestroyTexture(texture);
}

sdl_texture::sdl_texture(sdl_render& render, sdl_surface& surf) :
    _render(render)
  , _texture(SDL_CreateTexture(render.get(), surf.foramt(), SDL_TEXTUREACCESS_STREAMING, surf.width(), surf.height()))
  , _pixels(NULL)
  , _pitch(0)
  , _width(surf.width())
  , _height(surf.height())
{

}

SDL_Texture*sdl_texture::get()
{
    return _texture.get();
}

const SDL_Texture*sdl_texture::get() const
{
    return _texture.get();
}

void sdl_texture::set_color(uint8_t red, uint8_t green, uint8_t blue)
{
    SDL_SetTextureColorMod(_texture.get(), red, green, blue);
}

void sdl_texture::set_blend_mode(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(_texture.get(), blending);
}

void sdl_texture::set_alpha(uint8_t alpha)
{
    SDL_SetTextureAlphaMod(_texture.get(), alpha);
}

int sdl_texture::width()
{
    return  _width;
}

int sdl_texture::height()
{
    return  _height;
}

void sdl_texture::lockTexture()
{
    if(_pixels != NULL)
    {
        throw sdl_exception("Texture is already locked!");
    }
    else
    {
        if( SDL_LockTexture(_texture.get(), NULL, &_pixels, &_pitch) != 0)
        {
            throw sdl_exception("Unable to lock texture!");
        }
    }
}

void sdl_texture::unlockTexture()
{
    if(_pixels == NULL)
    {
        throw sdl_exception("Texture is not locked!");
    }
    else
    {
        SDL_UnlockTexture(_texture.get());
        _pixels = NULL;
        _pitch = 0;
    }
}

void*sdl_texture::pixels()
{
    return _pixels;
}

int sdl_texture::pitch()
{
    return _pitch;
}

uint32_t& sdl_texture::at(int x, int y)
{
    return *(reinterpret_cast<uint32_t*>(((uint8_t*)_pixels) + (_height - y)*_pitch + x*sizeof(uint32_t)));
}

void sdl_texture::render()
{
    SDL_SetRenderDrawColor(_render.get(), 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear(_render.get());
    SDL_RenderCopy(_render.get(), _texture.get(), NULL, NULL);
    SDL_RenderPresent(_render.get());
}

void sdl_texture::render(int x, int y, int width, int height)
{
    SDL_Rect renderQuad = { x, y, width, height };

    SDL_RenderCopyEx(_render.get(), _texture.get(), NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE );
}
