#include <chrono>

using Clock = std::chrono::high_resolution_clock;


class Timer
{
private:
    Clock::time_point start_time, stop_time;

public:
    Timer();

    void start();
 
    double getElapsedTimeInSeconds();

    double stop();

};