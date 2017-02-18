#ifndef TEXT_HPP
#define TEXT_HPP

#include <memory>
#include <string>

#include <SDL2/SDL_ttf.h>

#include "sdl/render/render.hpp"
#include "sdl/texture/texture.hpp"
#include "sdl/surface/surface.hpp"

class sdl_text
{
    class font_deleter
    {
    public:
        void operator()(TTF_Font* font);
    };
public:
    sdl_text();

    void print(const std::string& text, const int& x, const int& y, sdl_render& render);

private:
    std::unique_ptr<TTF_Font, font_deleter> _font;
};

#endif //TEXT_HPP
