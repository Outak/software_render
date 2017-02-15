#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <memory>

#include <SDL2/SDL.h>

#include "sdl/sdl.hpp"

#include "geometry.h"

class model
{
public:
    model(const std::string& file_name)
    {
        std::ifstream file(file_name);
        if (!file.good())
        {
            throw std::runtime_error(std::string("can't open model file: ") + file_name);
        }
        std::string line;
        while (!file.eof()) {
            std::getline(file, line);
            std::istringstream iss(line.c_str());
            char trash;
            if (!line.compare(0, 2, "v ")) {
                iss >> trash;
                Vec3f v;
                for (int i=0;i<3;i++) iss >> v.raw[i];
                _vertexes.push_back(v);
            } else if (!line.compare(0, 2, "f ")) {
                std::array<int,3> f;
                int itrash, idx;
                iss >> trash; int i = 0;
                while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                    f[i++] = idx - 1;
                }
                _faces.push_back(f);
            }
        }
        std::cout << "# v# " << _vertexes.size() << " f# "  << _faces.size() << std::endl;
    }

    int nverts() {
        return (int)_vertexes.size();
    }

    int nfaces() {
        return (int)_faces.size();
    }

    std::array<int, 3> face(int idx) {
        return _faces[idx];
    }

    Vec3f vert(int i) {
        return _vertexes[i];
    }

private:
    std::vector<Vec3f> _vertexes;
    std::vector<std::array<int, 3>> _faces;
};

class render
{
public:
    static void line(int x0, int y0, int x1, int y1, sdl_texture& image, const uint32_t& color)
    { // Bresenham's line algorithm
        bool steep = false;
        if (std::abs(x0-x1) < std::abs(y0-y1))
        {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0>x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        for (int x=x0, y = y0, y_error = 0, dy_error = 2*std::abs(y1 - y0), dx_error = (x1-x0); x<=x1; x++, y_error += dy_error)
        {
            if (steep) {
                image.at(y, x) = color; // if transposed, de-transpose
            } else {
                image.at(x, y) = color;
            }
            if (y_error > dx_error)
            {
                 y += (y1 > y0) ? 1 : -1;
                 y_error -= 2*dx_error;
            }
        }
    }

    static void line(Vec2i st, Vec2i fn, sdl_texture& image, const uint32_t& color)
    {
        line(st.x, st.y, fn.x, fn.y, image, color);
    }

    static void triangle(std::array<Vec2i, 3> vertexes, sdl_texture& image, const uint32_t& color)
    {
        std::sort(vertexes.begin(), vertexes.end(), [](const Vec2i& l, const Vec2i& r) {return l.y < r.y;});
        Vec2i& bottom = vertexes[0];
        Vec2i& mid = vertexes[1];
        Vec2i& top = vertexes[2];

        int l0 = top.y - bottom.y;
        int l1 = mid.y - bottom.y + 1;
        for(int y = bottom.y; y <= mid.y; ++y)
        {
            float k1 = (float)(y - bottom.y)/l0;
            float k2  = (float)(y - bottom.y)/l1;
            Vec2i left = bottom + (top - bottom)*k1;
            Vec2i right = bottom + (mid - bottom)*k2;
            if (left.x > right.x) std::swap(left, right);
            for(int x = left.x; x < right.x; ++x)
            {
                image.at(x,y) = color;
            }
        }
        l1 = top.y - mid.y + 1;
        for(int y = mid.y; y <= top.y; ++y)
        {
            float k1 = (float)(y - bottom.y)/l0;
            float k2  = (float)(y - mid.y)/l1;
            Vec2i left = bottom + (top - bottom)*k1;
            Vec2i right = mid + (top - mid)*k2;
            if (left.x > right.x) std::swap(left, right);
            for(int x = left.x; x < right.x; ++x)
            {
                image.at(x,y) = color;
            }
        }
    }

    static void mesh(model& m, sdl_texture& image, const uint32_t& color)
    {
        for (int i=0; i<m.nfaces(); i++)
        {
            std::array<int, 3> face = m.face(i);
            for (int j=0; j<3; j++) {
                Vec3f v0 = m.vert(face[j]);
                Vec3f v1 = m.vert(face[(j+1)%3]);
                int x0 = (v0.x+1.)*image.width()/2.;
                int y0 = (v0.y+1.)*image.height()/2.;
                int x1 = (v1.x+1.)*image.width()/2.;
                int y1 = (v1.y+1.)*image.height()/2.;
                line(x0, y0, x1, y1, image, color);
            }
        }
    }

    static void surf(model& m, sdl_texture& image, sdl_surface& screen_surface, Vec3f light_dir)
    {
        for (int i = 0; i < m.nfaces(); ++i)
        {
            std::array<int, 3> face = m.face(i);
            std::array<Vec2i,3> screen_coords;
            std::array<Vec3f,3> world_coords;
            for (int j=0; j<3; j++)
            {
                Vec3f v = m.vert(face[j]);
                screen_coords[j] = Vec2i((v.x+1.)*image.width()/2., (v.y+1.)*image.height()/2.);
                world_coords[j]  = v;
            }
            Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
            n.normalize();
            float intensity = n*light_dir;
            if (intensity>0) {
                triangle(screen_coords, image, SDL_MapRGB(screen_surface.pix_foramt(), intensity*255, intensity*255, intensity*255));
            }
        }
    }
};

class render_context : public sdl_context
{
public:
    render_context() :
        main_window("HABR_RENDER", 1024, 1024)
      , main_render(main_window)
      , screen_surface(main_window.surface())
      , screen_texture(main_render, screen_surface)
      , head_model("../software_render/head.obj")
    {

    }

    void loop() override
    {
        screen_texture.lockTexture();

        //render::mesh(head_model, screen_texture, SDL_MapRGB(screen_surface.pix_foramt(), 0x00, 0xff, 0x00));
        Vec3f light_dir(0.01,0,-1);
        render::surf(head_model, screen_texture, screen_surface, light_dir);

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
