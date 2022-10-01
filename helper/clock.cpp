#include "clock.h"


Timer::Timer()
{
    start();
}

void Timer::start()
{
    start_time = Clock::now();
}

double Timer::getElapsedTimeInSeconds()
{
    return std::chrono::duration<double>(stop_time - start_time).count();
}

double Timer::stop()
{
    stop_time = Clock::now();
    return getElapsedTimeInSeconds();
}
