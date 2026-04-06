#pragma once
#include <chrono>

class Timer {
private:
    using clock = std::chrono::steady_clock;
    clock::time_point lastTime;

public:
    Timer();
    float restart();
};