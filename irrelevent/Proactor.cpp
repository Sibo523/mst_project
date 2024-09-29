// src/utils/Proactor.cpp
#include "Proactor.hpp"

Proactor::Proactor(size_t numThreads) : stop(false) {
    for(size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(
            [this] {
                this->workerThread();
            }
        );
    }
}

Proactor::~Proactor() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers) {
        worker.join();
    }
}

void Proactor::workerThread() {
    while(true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            this->condition.wait(lock,
                [this]{ return this->stop || !this->tasks.empty(); });
            if(this->stop && this->tasks.empty())
                return;
            task = std::move(this->tasks.front());
            this->tasks.pop();
        }
        task();
    }
}
