#include <Timer.h>

Timer::Timer() : lastTime(clock::now()) {}

float Timer::restart() {
    clock::time_point currentTime = clock::now();
    std::chrono::duration<float> delta = currentTime - lastTime;
    lastTime = currentTime;
    return delta.count();
}