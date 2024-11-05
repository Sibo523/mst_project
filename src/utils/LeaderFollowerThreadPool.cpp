// LeaderFollowerThreadPool.hpp remains the same

// Update LeaderFollowerThreadPool.cpp:
#include "LeaderFollowerThreadPool.hpp"

void LeaderFollowerThreadPool::workerThread(size_t id)
{
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this, id]
                           { return stop || (!tasks.empty() && leader == id); });

            if (stop && tasks.empty())
            {
                --active_threads;
                return;
            }

            if (!tasks.empty() && leader == id)
            {
                task = std::move(tasks.front());
                tasks.pop();
                leader = (id + 1) % workers.size();
                condition.notify_all();
            }
        }

        if (task)
        {
            task();
        }
    }
}

std::string LeaderFollowerThreadPool::processMST(const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    if (mst.empty())
    {
        return "Error: Empty MST provided";
    }

    // Create a single future for the getAllResults task
    auto future = enqueue([&mst]() {
        return MSTAnalysis::getAllResults(mst);
    });

    try
    {
        // Get the result string directly
        return future.get();
    }
    catch (const std::exception &e)
    {
        return std::string("Error in analysis: ") + e.what();
    }
}