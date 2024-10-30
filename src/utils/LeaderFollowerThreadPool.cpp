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

    std::stringstream result;
    std::vector<std::future<std::string>> futures;

    // Enqueue MST analysis tasks
    futures.push_back(enqueue([&mst]()
                              { return "Total Weight: " + std::to_string(MSTAnalysis::calculateTotalWeight(mst)); }));

    futures.push_back(enqueue([&mst]()
                              { return "Longest Distance: " + std::to_string(MSTAnalysis::findLongestDistance(currentGraph, mst)); }));

    futures.push_back(enqueue([&mst]()
                              { return "Average Distance: " + std::to_string(MSTAnalysis::calculateAverageDistance(currentGraph, mst)); }));

    futures.push_back(enqueue([&mst]()
                              { return "Shortest Edge: " + std::to_string(MSTAnalysis::findShortestMSTEdge(mst)); }));

    // Collect results
    for (auto &future : futures)
    {
        try
        {
            result << future.get() << "\n";
        }
        catch (const std::exception &e)
        {
            result << "Error in analysis: " << e.what() << "\n";
        }
    }

    return result.str();
}

template <class F>
auto enqueue(F &&f) -> std::future<typename std::result_of<F()>::type>
{
    using return_type = typename std::result_of<F()>::type;

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
