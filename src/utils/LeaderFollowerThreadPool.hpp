#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <sstream>

class LeaderFollowerThreadPool
{
public:
    LeaderFollowerThreadPool(size_t numThreads)
        : stop(false), leader(0), active_threads(numThreads)
    {
        for (size_t i = 0; i < numThreads; ++i)
        {
            workers.emplace_back([this, i]
                                 { this->workerThread(i); });
        }
    }

    ~LeaderFollowerThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (auto &worker : workers)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }

    // Main method to process MST results
    std::string processMST(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
   
private:
    void workerThread(size_t id);

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
    std::atomic<size_t> leader;
    std::atomic<size_t> active_threads;
};