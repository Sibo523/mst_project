
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
#include <string>
#include "../analysis/MSTAnalysis.hpp"

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

    // Template function for enqueueing tasks
    template <class F>
    auto enqueue(F &&f) -> std::future<typename std::invoke_result<F>::type>
    {                                                             // Updated to use invoke_result
        using return_type = typename std::invoke_result<F>::type; // Updated from result_of

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(f));
        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop)
            {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            tasks.emplace([task]()
                          { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    // Main method to process MST results
    std::string processMST(const std::vector<std::pair<int, std::pair<int, int>>> &mst);

    // Assignment operator
    LeaderFollowerThreadPool &operator=(const LeaderFollowerThreadPool &other) = delete; // Better to delete it
    LeaderFollowerThreadPool(const LeaderFollowerThreadPool &other) = delete;            // Delete copy constructor too

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
