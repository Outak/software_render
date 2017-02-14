#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <memory>

#include <SDL2/SDL.h>

#include "geometry.h"

class sdl_exception : std::exception
{
public:
    sdl_exception(const std::string& msg):
        _msg(msg + " SDL_Error : " + SDL_GetError())
    {}

private:
    std::string _msg;
};

class sdl_context
{
public:
    virtual ~sdl_context() {}
    virtual void loop() = 0;
};

class sdl_system
{
public:
    sdl_system() :
        _work(true)
    {
        if( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
        {
            throw sdl_exception("SDL could not initialize!");
        }
    }

    ~sdl_system()
    {
        SDL_Quit();
    }

    void loop(sdl_context& context)
    {
        SDL_Event event;
        while(_work)
        {
            while(SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    _work = false;
                }
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        _work = false;
                    }
                }
            }

            context.loop();
        }
    }

private:
    bool _work;
};

class texture;

class window
{
    class widow_deleter
    {
    public:
        void operator()(SDL_Window* window)
        {
            SDL_DestroyWindow(window);
        }
    };

    class render_deleter
    {
    public:
        void operator()(SDL_Renderer* render)
        {
            SDL_DestroyRenderer(render);
        }
    };

public:
    window(const std::string& name, const int& w, const int& h) :
        _window(SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN))
      , _render(SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED))
    {
        if(!_window)
        {
            throw sdl_exception("Window could not be created!");
        }
    }

    SDL_Window* get()
    {
        return _window.get();
    }

    SDL_Surface* surface()
    {
        return SDL_GetWindowSurface(_window.get());
    }

    SDL_Renderer* renderer()
    {
        return _render.get();
    }

    void update()
    {
        SDL_UpdateWindowSurface(_window.get());
    }

private:
    std::unique_ptr<SDL_Window, widow_deleter> _window;
    std::unique_ptr<SDL_Renderer, render_deleter> _render;
};

class surface
{
public:
    virtual SDL_Surface* get() = 0;
    virtual int width() = 0;
    virtual int height() = 0;
    virtual SDL_PixelFormat* pix_foramt() = 0;
    virtual uint32_t foramt() = 0;
};

class surface_store : public surface
{
    class surface_deleter
    {
    public:
        void operator()(SDL_Surface* surface)
        {}
    };

public:
    surface_store(SDL_Surface* surface0) :
        _surface(surface0)
    {

    }

    SDL_Surface* get() override
    {
        return _surface.get();
    }

    int width() override
    {
        return _surface.get()->w;
    }

    int height() override
    {
        return _surface.get()->h;
    }

    virtual uint32_t foramt() override
    {
        return _surface.get()->format->format;
    }

    SDL_PixelFormat* pix_foramt() override
    {
        return _surface.get()->format;
    }

private:
    std::unique_ptr<SDL_Surface, surface_deleter> _surface;
};

class surface_view : public surface
{
    class empty_deleter
    {
    public:
        void operator()(SDL_Surface* surface)
        {}
    };

public:
    surface_view(SDL_Surface* surface) :
        _surface(surface)
    {

    }

    SDL_Surface* get() override
    {
        return _surface.get();
    }

    int width() override
    {
        return _surface.get()->w;
    }

    int height() override
    {
        return _surface.get()->h;
    }

    virtual uint32_t foramt() override
    {
        return _surface.get()->format->format;
    }

    SDL_PixelFormat* pix_foramt() override
    {
        return _surface.get()->format;
    }

private:
    std::unique_ptr<SDL_Surface, empty_deleter> _surface;
};

class texture
{
    class texture_deleter
    {
    public:
        void operator()(SDL_Texture* texture)
        {
            SDL_DestroyTexture(texture);
        }
    };

public:
    texture(window& target_window, surface& surf) :
        _texture(SDL_CreateTexture(target_window.renderer(), surf.foramt(), SDL_TEXTUREACCESS_STREAMING, surf.width(), surf.height()))
      , _pixels(NULL)
      , _pitch(0)
      , _width(surf.width())
      , _height(surf.height())
    {

    }

    SDL_Texture* get()
    {
        return _texture.get();
    }

    void render(window& target_window)
    {
        SDL_SetRenderDrawColor( target_window.renderer(), 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(target_window.renderer());
        SDL_RenderCopy(target_window.renderer(), _texture.get(), NULL, NULL);
        SDL_RenderPresent(target_window.renderer());
    }

    void set_color(uint8_t red, uint8_t green, uint8_t blue)
    {
        SDL_SetTextureColorMod(_texture.get(), red, green, blue);
    }

    void set_blend_mode(SDL_BlendMode blending)
    {
        SDL_SetTextureBlendMode(_texture.get(), blending);
    }

    void set_alpha(uint8_t alpha)
    {
        SDL_SetTextureAlphaMod(_texture.get(), alpha);
    }

    //Renders texture at given point
    void render(window& target_window, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE)
    {
        SDL_Rect renderQuad = { x, y, _width, _height };

        if( clip != NULL )
        {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }

        SDL_RenderCopyEx(target_window.renderer(), _texture.get(), clip, &renderQuad, angle, center, flip );
    }

    int width()
    {
        return  _width;
    }

    int height()
    {
        return  _height;
    }

    //Pixel manipulators
    void lockTexture()
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

    void unlockTexture()
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

    void* pixels()
    {
        return _pixels;
    }

    int pitch()
    {
        return _pitch;
    }

    uint32_t& at(int x, int y)
    {
        return *(reinterpret_cast<uint32_t*>(((uint8_t*)_pixels) + (_height - y)*_pitch + x*sizeof(uint32_t)));
    }

private:
    std::unique_ptr<SDL_Texture, texture_deleter> _texture;
    void* _pixels;
    int _pitch;
    int _width;
    int _height;
};

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
    static void line(int x0, int y0, int x1, int y1, texture& image, const uint32_t& color)
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

    static void line(Vec2i st, Vec2i fn, texture& image, const uint32_t& color)
    {
        line(st.x, st.y, fn.x, fn.y, image, color);
    }

    static void triangle(std::array<Vec2i, 3> vertexes, texture& image, const uint32_t& color)
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

    static void mesh(model& m, texture& image, const uint32_t& color)
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

    static void surf(model& m, texture& image, surface_view& screen_surface, Vec3f light_dir)
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
      , screen_surface(main_window.surface())
      , screen_texture(main_window, screen_surface)
      , head_model("../software_render/head.obj")
    {

    }

    void loop() override
    {
        screen_texture.lockTexture();

        //render::mesh(head_model, screen_texture, SDL_MapRGB(screen_surface.pix_foramt(), 0x00, 0xff, 0x00));
        Vec3f light_dir(0,0,-1);
        render::surf(head_model, screen_texture, screen_surface, light_dir);

        screen_texture.unlockTexture();

        screen_texture.render(main_window);

        SDL_Delay(40);
    }

private:
    window main_window;
    surface_view screen_surface;
    texture screen_texture;
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
