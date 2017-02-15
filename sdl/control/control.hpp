#ifndef CONTROL_HPP
#define CONTROL_HPP

class sdl_control
{
public:
    sdl_control();
    ~sdl_control();

    bool is_work() const;
    void process();

private:
    bool _is_work;
};

#endif // CONTROL_HPP
