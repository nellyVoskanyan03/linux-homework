#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

typedef void (*pool_function_t)(void*);

class parallel_scheduler {
public:
    parallel_scheduler(size_t capacity);
    ~parallel_scheduler();

    void run(pool_function_t func, void* arg);

private:
    void worker_thread();

    size_t _capacity;
    std::vector<std::thread> _threads;
    std::queue<std::pair<pool_function_t, void*>> task_queue;
    std::mutex queue_mutex;
    std::condition_variable _condition;
    bool _stop;
};

#endif
