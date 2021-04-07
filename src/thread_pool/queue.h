#ifndef _QUEUE_HPP
#define _QUEUE_HPP
#include <queue>
#include <mutex>
#include <optional>

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
        std::optional<T> Pop();

        size_t Size() const {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.size();
        }

        bool Empty() const {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.empty();
        }
    };
}

#include "queue.tpl"

#endif