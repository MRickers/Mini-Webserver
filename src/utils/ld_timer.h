#include <chrono>

namespace ld_utils {
    class Timer {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> _start;
        std::chrono::time_point<std::chrono::high_resolution_clock> _end;

    public:
        Timer(const Timer&) = delete;
        Timer operator=(const Timer&) = delete;

        Timer(std::chrono::time_point<std::chrono::high_resolution_clock> start);
        Timer();
        
        void Start();
        double Stop();
        double Round() const;

        double Time() const;
    };
}