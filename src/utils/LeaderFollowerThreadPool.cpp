// src/utils/LeaderFollowerThreadPool.cpp
#include "LeaderFollowerThreadPool.hpp"

LeaderFollowerThreadPool::LeaderFollowerThreadPool(size_t numThreads)
    : stop(false), leader(0)
{
    for(size_t i = 0; i < numThreads; ++i)
        workers.emplace_back(
            [this, i]{ this->workerThread(i); }
        );
}

LeaderFollowerThreadPool::~LeaderFollowerThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}

void LeaderFollowerThreadPool::workerThread(size_t id)
{
    while(true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            
            if (this->leader == id) {
                this->condition.wait(lock,
                    [this]{ return this->stop || !this->tasks.empty(); });
                if(this->stop && this->tasks.empty())
                    return;
                task = std::move(this->tasks.front());
                this->tasks.pop();
                this->leader = (id + 1) % this->workers.size();
                this->condition.notify_all();
            } else {
                this->condition.wait(lock,
                    [this, id]{ return this->stop || this->leader == id; });
                if(this->stop && this->tasks.empty())
                    return;
            }
        }
        if (task) {
            task();
        }
    }
}