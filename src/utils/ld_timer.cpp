#include "ld_timer.h"

namespace ld_utils {

    Timer::Timer(std::chrono::time_point<std::chrono::high_resolution_clock> start) :
    _start(start), 
    _end() {

    }

    Timer::Timer() :
    _start(),
    _end() {

    }

    void Timer::Start() {
        _start = std::chrono::high_resolution_clock::now();
    }

    double Timer::Stop() {
        _end = std::chrono::high_resolution_clock::now();
        return Time();
    }

    double Timer::Round() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - _start).count();
    }

    double Timer::Time() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count();
    }
}