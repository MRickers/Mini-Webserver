#include "thread_pool.h"

namespace thread_pool {
    ThreadPool::ThreadPool(
        size_t thread_count=std::thread::hardware_concurrency(),
        Logger logger
    ) :
    _queue(),
    _threads(),
    _thread_count(thread_count),
    _threads_running(0),
    _running(true),
    _mutex(),
    _cond(),
    _logger(logger) {
        logger->Debug("Initializing threadpool for "+std::to_string(_thread_count)+" threads.");
    }

    void ThreadPool::worker() {
        std::unique_lock<std::mutex> lock(_mutex);

        while(_running) {
            _cond.wait(lock);
        }
    }
    void Push(const job& j);
    void Terminate();
    size_t WorkingThreads() const;
    void Pause();
    void Resume();
}