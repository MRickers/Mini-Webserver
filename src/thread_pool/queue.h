#ifndef _QUEUE_HPP
#define _QUEUE_HPP
#include <queue>
#include <mutex>

namespace queue {
    template<typename T>
    class BlockingQueue {
    private:
        std::queue<T> _queue;
        mutable std::mutex _mutex;

    public:
        BlockingQueue(const BlockingQueue&) = delete;
        BlockingQueue& operator=(const BlockingQueue&) = delete;
        BlockingQueue() = default;
        
        void Push(const T& data);
        T Pop();

        size_t Size() const {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.size();
        }
    };
}

#include "queue.tpl"

#endif