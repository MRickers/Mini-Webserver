#include "thread_pool.h"
#include "logging_manager.h"

void func1() {
    auto logger = LogManager::GetLogger("func1");
    logger->Debug(STREAM("Func1 waiting 2000 ms"));
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}


void func2() {
    auto logger = LogManager::GetLogger("func2");
    logger->Debug(STREAM("Func1 waiting 5000 ms"));
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

int func3(int a) {
    return a + 1;
}

int main() {
    auto logging = LogManager::GetLogger("Main");
    {
        int a = 1;
        thread_pool::ThreadPool pool{LogManager::GetLogger("ThreadPool")};

        std::this_thread::sleep_for(std::chrono::seconds(2));
        pool.Push(func1);
        pool.Push(func2);
        pool.Push([a, logging] {
            int b = func3(a);
            logging->Debug(STREAM("b="<<b));
        });

        logging->Debug("Scope down");
    }
    logging->Debug("Main thread shutdown");
}