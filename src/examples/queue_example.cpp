#include "queue.h"
#include "logging_manager.h"
#include <functional>
#include <thread>

typedef std::function<void(Logger logger)> job;

void test1(Logger logger) {
    logger->Debug("Hello from test1");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void test2(Logger logger) {
    logger->Debug("Hello from test2");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void test3(Logger logger) {
    logger->Debug("Hello from test3");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int main() {
    const auto logger = LogManager::GetLogger("Main");

    queue::BlockingQueue<job> q;

    q.Push(test1);
    q.Push(test2);
    q.Push(test3);
    
    const auto size = q.Size();
    for(int i=0;i<size;i++) {
        const auto j = q.Pop();
        j(logger);
    }

    logger->Debug("Cya");
}