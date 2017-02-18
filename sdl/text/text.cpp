#include "text.hpp"

#include "sdl/common/common.hpp"

void sdl_text::font_deleter::operator()(TTF_Font* font)
{
    TTF_CloseFont(font);
}

sdl_text::sdl_text() :
    _font(TTF_OpenFont("../software_render/VCR_OSD_MONO.ttf", 14))
{
    if(!_font)
    {
        throw sdl_exception("could not open ttf file");
    }
}

void sdl_text::print(const std::string& text, const int& x, const int& y, sdl_render& render)
{
    sdl_surface_store text_surface(TTF_RenderText_Solid(_font.get(), text.c_str(), SDL_Color({0xff, 0xff, 0xff, 0x00})));
    sdl_texture texture(render, text_surface);
    texture.render(x, y, text_surface.width(), text_surface.height());
}
