#include "thread_pool.h"

namespace thread_pool {
    ThreadPool::ThreadPool(
        Logger logger,
        size_t thread_count
    ) :
    _queue(),
    _threads(),
    _thread_count(thread_count),
    _threads_running(0),
    _paused(false),
    _running(true),
    _mutex(),
    _cond(),
    _logger(logger) {

        for(int i=0;i<_thread_count;++i) {
            _threads.push_back(std::thread(&ThreadPool::worker, this));
        }
        logger->Debug("Initializing threadpool for "+std::to_string(_thread_count)+" threads.");
    }

    ThreadPool::~ThreadPool() {
        Terminate();
        for(auto& t : _threads) {
            if(t.joinable()) {
               t.join();
            }
        }
        _logger->Debug("Shutting down");
    }

    void ThreadPool::worker() {
        while(_running) {
            if(!_running) {
                break;
            }
            workwork();

            if(!_running) {
                break;
            }

            const auto task = _queue.Pop();

            if(task.has_value()) {
                const auto func = task.value();
                func();
            }
            
        }
    }

    void ThreadPool::workwork() {
        std::unique_lock<std::mutex> lock(_mutex);
        // if queue is empty or pool is paused go wait
        if(_queue.Empty() || _paused) {
            _cond.wait(lock, [&] {
                return !_running || !(_queue.Empty() || _paused);
            });
        }
    }

    void ThreadPool::Push(const job& j) {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.Push(j);
        _cond.notify_one();
    }

    void ThreadPool::Terminate() {
        std::lock_guard<std::mutex> lock(_mutex);
        while(!_queue.Empty()) {
            _queue.Pop();
        }
        _running = false;
        _cond.notify_all();
    }

    size_t ThreadPool::WorkingThreads() const {
        return _threads.size();
    }

    void ThreadPool::Pause(bool state) {
        std::lock_guard<std::mutex> lock(_mutex);
        _paused = state;
    }
}