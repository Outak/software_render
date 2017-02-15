#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <memory>

struct SDL_Surface;
struct SDL_PixelFormat;

class sdl_surface
{
public:
    virtual SDL_Surface* get() = 0;
    virtual int width() = 0;
    virtual int height() = 0;
    virtual SDL_PixelFormat* pix_foramt() = 0;
    virtual uint32_t foramt() = 0;
};

class sdl_surface_store : public sdl_surface
{
    class surface_deleter
    {
    public:
        void operator()(SDL_Surface* surface);
    };

public:
    sdl_surface_store(SDL_Surface* surface0);

    SDL_Surface* get() override;
    int width() override;
    int height() override;
    SDL_PixelFormat* pix_foramt() override;
    virtual uint32_t foramt() override;

private:
    std::unique_ptr<SDL_Surface, surface_deleter> _surface;
};

class sdl_surface_view : public sdl_surface
{
    class empty_deleter
    {
    public:
        void operator()(SDL_Surface* surface);
    };

public:
    sdl_surface_view(SDL_Surface* surface);

    SDL_Surface* get() override;
    int width() override;
    int height() override;
    SDL_PixelFormat* pix_foramt() override;
    virtual uint32_t foramt() override;

private:
    std::unique_ptr<SDL_Surface, empty_deleter> _surface;
};

#endif // SURFACE_HPP
