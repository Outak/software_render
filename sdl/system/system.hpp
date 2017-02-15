#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "sdl/control/control.hpp"

class sdl_context
{
public:
    virtual ~sdl_context() {}
    virtual void loop() = 0;
};

class sdl_system
{
public:
    sdl_system();

    ~sdl_system();

    void loop(sdl_context& work_context);

private:
    sdl_control _control;
};

#endif // SYSTEM_HPP
