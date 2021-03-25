#ifndef _THREAD_POOL_HPP
#define _THREAD_POOL_HPP
#include <functional>
#include <condition_variable>
#include <thread>
#include <vector>
#include <atomic>
#include "queue.h"
#include "logging_manager.h"

namespace thread_pool {
    typedef std::function<void()> job;

    class ThreadPool {
        private:
            queue::BlockingQueue<job> _queue;
            std::vector<std::thread> _threads;
            std::atomic<size_t> _thread_count;
            std::atomic<size_t> _threads_running;
            bool _running;
            std::mutex _mutex;
            std::condition_variable _cond;
            Logger _logger;

            void worker();
        public:
            ThreadPool(const ThreadPool&) = delete;
            ThreadPool& operator=(const ThreadPool&) = delete;

            ThreadPool(size_t thread_count=std::thread::hardware_concurrency(), Logger logger);

            void Push(const job& j);
            void Terminate();
            size_t WorkingThreads() const;
            void Pause();
            void Resume();
    };
}



#endif // _THREAD_POOL_HPP