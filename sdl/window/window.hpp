#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <memory>

#include "sdl/surface/surface.hpp"

struct SDL_Window;

class sdl_window
{
    class widow_deleter
    {
    public:
        void operator()(SDL_Window* sdl_window);
    };

public:
    sdl_window(const std::string& name, const int& w, const int& h);

    SDL_Window* get();

    const SDL_Window* get() const;

    sdl_surface_view surface();

    void update();

private:
    std::unique_ptr<SDL_Window, widow_deleter> _window;
};


#endif // WINDOW_HPP
