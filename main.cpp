#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>

#include "sdl/sdl.hpp"
#include "geometry/geometry.hpp"
#include "file_system/wavefront_obj.hpp"
#include "model/model.hpp"
#include "software_render/software_render.hpp"

class test_context : public sdl_context
{
public:
    test_context() :
        main_window("HABR_RENDER", 1024, 1024)
      , main_render(main_window)
      , screen_surface(main_window.surface())
      , screen_texture(main_render, screen_surface)
    {
        std::ifstream mfile("../software_render/head.obj");
        head_model = wavefront_obj::read_model(mfile);
    }

    void loop() override
    {
        screen_texture.lockTexture();

        std::array<Vec2i, 3> t0 = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
        std::array<Vec2i, 3> t1 = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
        std::array<Vec2i, 3> t2 = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};


        render::triangle(t0, screen_texture, SDL_MapRGB(screen_surface.pix_foramt(), 0x00, 0xff, 0x00));
        render::triangle(t1, screen_texture, SDL_MapRGB(screen_surface.pix_foramt(), 0x00, 0x00, 0xff));
        render::triangle(t2, screen_texture, SDL_MapRGB(screen_surface.pix_foramt(), 0xff, 0x00, 0x00));

        screen_texture.unlockTexture();

        screen_texture.render();

        SDL_Delay(40);
    }

private:
    sdl_window main_window;
    sdl_render main_render;
    sdl_surface_view screen_surface;
    sdl_texture screen_texture;
    model head_model;
};

class render_context : public sdl_context
{
public:
    render_context() :
        main_window("HABR_RENDER", 1024, 1024)
      , main_render(main_window)
      , screen_surface(main_window.surface())
      , screen_texture(main_render, screen_surface)
    {
        std::ifstream mfile("../software_render/head.obj");
        head_model = wavefront_obj::read_model(mfile);
    }

    void loop() override
    {
        screen_texture.lockTexture();
        Vec3f light_dir(0.00,0,-1);

        render::surf(head_model, screen_texture, screen_surface, light_dir);

        //render::mesh(head_model, screen_texture, SDL_MapRGB(screen_surface.pix_foramt(), 0x00, 0xff, 0x00));

        screen_texture.unlockTexture();

        screen_texture.render();

        SDL_Delay(40);
    }

private:
    sdl_window main_window;
    sdl_render main_render;
    sdl_surface_view screen_surface;
    sdl_texture screen_texture;
    model head_model;
};

int main(int argc, char *argv[])
{
    try
    {
        sdl_system sdl;
        render_context r;

        sdl.loop(r);
    }
    catch(const std::exception& exp)
    {
        std::cout << exp.what() << std::endl;
    }

    return 0;
}
