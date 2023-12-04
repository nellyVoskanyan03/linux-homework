#include "parallel_scheduler.h"

extern "C" parallel_scheduler* create_parallel_scheduler(size_t capacity) {
    return new parallel_scheduler(capacity);
}

parallel_scheduler::parallel_scheduler(size_t capacity) : _capacity(capacity), _stop(false) {
    for (int i = 0; i < _capacity; ++i) {
        _threads.emplace_back(&parallel_scheduler::worker_thread, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    _stop = true;
    _condition.notify_all();
    lock.unlock();

    for (std::thread& thread : _threads) {
        thread.join();
    }
}


void parallel_scheduler::run(pool_function_t func, void* arg) {
    std::unique_lock<std::mutex> lock(queue_mutex);
    auto args = std::make_pair(func, arg);
    task_queue.emplace(args);
    _condition.notify_one();
}

void parallel_scheduler::worker_thread() {
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);

        _condition.wait(lock, [this] { return _stop || !task_queue.empty(); });

        if (_stop && task_queue.empty()) {
            return;
        }

        auto task = task_queue.front();
        task_queue.pop();

        lock.unlock();

        task.first(task.second);
    }
}
