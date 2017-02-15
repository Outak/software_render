#include "system.hpp"

#include "sdl/common/common.hpp"

#include <SDL2/SDL.h>

sdl_system::sdl_system()
{
    if( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
    {
        throw sdl_exception("SDL could not initialize!");
    }
}

sdl_system::~sdl_system()
{
    SDL_Quit();
}

void sdl_system::loop(sdl_context& work_context)
{
    while(_control.is_work())
    {
        _control.process();
        work_context.loop();
    }
}
