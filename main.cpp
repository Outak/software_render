#include <iostream>
#include <stdexcept>
#include <memory>

#include <SDL2/SDL.h>

class sdl_exception : std::exception
{
public:
    sdl_exception(const std::string& msg):
        _msg(msg + " SDL_Error : " + SDL_GetError())
    {}

private:
    std::string _msg;
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

    template<class Process>
    void loop(Process&& process)
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

            process();
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
        return *(reinterpret_cast<uint32_t*>(((uint8_t*)_pixels) + y*_pitch + x*sizeof(uint32_t)));
    }

private:
    std::unique_ptr<SDL_Texture, texture_deleter> _texture;
    void* _pixels;
    int _pitch;
    int _width;
    int _height;
};

class render
{
public:
    static void line(int x0, int y0, int x1, int y1, texture& image, const uint32_t& color) {
        bool steep = false;
        if (std::abs(x0-x1)<std::abs(y0-y1)) { // if the line is steep, we transpose the image
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0>x1) { // make it left-to-right
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        for (int x=x0; x<=x1; x++) {
            float t = (x-x0)/(float)(x1-x0);
            int y = y0*(1.-t) + y1*t;
            if (steep) {
                image.at(y, x) = color;
            } else {
                image.at(x, y) = color;
            }
        }
    }
};

int main(int argc, char *argv[])
{
    try
    {
        sdl_system sdl;
        window main_window("HABR_RENDER", 640, 480);
        surface_view screen_surface(main_window.surface());
        texture screen_texture(main_window, screen_surface);

        sdl.loop([&]() {
            screen_texture.lockTexture();

            render::line(13, 20, 80, 40, screen_texture, SDL_MapRGB(screen_surface.pix_foramt(), 0x00, 0xff, 0x00));
            render::line(20, 13, 40, 80, screen_texture, SDL_MapRGB(screen_surface.pix_foramt(), 0x00, 0xff, 0x00));
            render::line(80, 40, 13, 20, screen_texture, SDL_MapRGB(screen_surface.pix_foramt(), 0xff, 0x00, 0x00));

            screen_texture.unlockTexture();

            screen_texture.render(main_window);

            SDL_Delay(40);
        });
    }
    catch(const std::exception& exp)
    {
        std::cout << exp.what() << std::endl;
    }

    return 0;
}
