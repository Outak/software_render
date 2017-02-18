#include "system.hpp"

#include <chrono>
#include <iostream>

using namespace std::chrono;
using hrc = high_resolution_clock;

#include "sdl/common/common.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

sdl_system::sdl_system()
{
    if( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
    {
        throw sdl_exception("SDL could not initialize!");
    }
    if( TTF_Init() < 0)
    {
        throw sdl_exception("SDL_ttf could not initialize!");
    }
}

sdl_system::~sdl_system()
{
    TTF_Quit();
    SDL_Quit();
}

void sdl_system::loop(sdl_context& work_context)
{
    bool started = false;
    double avg_time = 0.0;
    while(_control.is_work())
    {
        _control.process();
        hrc::time_point start = hrc::now();

        work_context.loop();
        hrc::time_point end = hrc::now();

        double current_duration = duration_cast<std::chrono::microseconds>(end - start).count();
        if (!started) { started = true; avg_time = current_duration; }
        avg_time = 0.9*avg_time + 0.1*current_duration;
        std::cout << "FPS: avg = " << 1.0e6/avg_time << " cur = " << 1.0e6 / current_duration << std::endl;
    }
}
