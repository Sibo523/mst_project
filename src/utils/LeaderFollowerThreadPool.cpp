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
    auto future = enqueue([&mst]()
                          { return MSTAnalysis::getAllResults(mst); });

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

/*
// If I didn't get what you want this what were before
  std::stringstream result;
    std::vector<std::future<std::pair<int, double>>> futures;

    // Enqueue MST analysis tasks
    futures.push_back(enqueue([&mst]()
                              { return MSTAnalysis::calculateTotalWeight(mst); }));

    futures.push_back(enqueue([&mst]()
                              { return MSTAnalysis::findLongestDistance(mst); }));

    futures.push_back(enqueue([&mst]()
                              { return MSTAnalysis::calculateAverageDistance(mst); }));

    futures.push_back(enqueue([&mst]()
                              { return MSTAnalysis::findShortestMSTEdge(mst); }));

    // Collect results
    std::vector<std::pair<std::string, double>> results = {
        {"Total Weight: ", 0.0},
        {"Longest Distance: ", 0.0},
        {"Average Distance: ", 0.0},
        {"Shortest Edge: ", 0.0}};

    // Collect and order results
    for (size_t i = 0; i < futures.size(); ++i)
    {
        try
        {
            auto [index, value] = futures[i].get();
            results[index].second = value;
        }
        catch (const std::exception &e)
        {
            result << "Error in analysis: " << e.what() << "\n";
        }
    }

    // Format results in order
    for (const auto &[label, value] : results)
    {
        result << label << value << "\n";
    }

    return result.str();
}
*/
