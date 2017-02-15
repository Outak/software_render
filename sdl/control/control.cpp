#include "control.hpp"

#include "SDL2/SDL_events.h"

sdl_control::sdl_control() :
    _is_work(true)
{

}

sdl_control::~sdl_control()
{
}

bool sdl_control::is_work() const
{
    return _is_work;
}

void sdl_control::process()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            _is_work = false;
        }
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                _is_work = false;
            }
        }
    }
}
