#include "Pipeline.hpp"
#include <iostream>
Pipeline::Pipeline(const std::vector<std::function<std::pair<int, double>(const std::vector<std::pair<int, std::pair<int, int>>> &)>> &functions)
{
    // Create active objects for each function
    for (const auto &function : functions)
    {
        activeObjects.push_back(std::make_unique<ActiveObject>(function));
    }
    for (int i = 0; i < 4; i++)
    {
        ans.push_back(0);
    }
}

Pipeline::~Pipeline()
{
    if (isStarted)
    {
        stop();
    }
}

void Pipeline::start()
{
    if (isStarted || activeObjects.empty())
        return;
    isStarted = true;
    std::cout << "we started start" << std::endl;
    // Start a thread for each active object
    for (size_t i = 0; i < activeObjects.size(); ++i)
    {
        auto &ao = activeObjects[i];

        ao->thread = std::thread([this, i]()
                                 {
            auto& current = *activeObjects[i];
            
            while (true) {
                std::vector<std::pair<int, std::pair<int, int>>> data;
                
                // Wait for and get data from queue
                {
                    std::unique_lock<std::mutex> lock(current.mutex);
                    current.condition.wait(lock, [&]() {
                        return !current.taskQueue.empty() || current.shouldStop;
                    });

                    if (current.shouldStop && current.taskQueue.empty()) {
                        break;
                    }

                    data = std::move(current.taskQueue.front());
                    current.taskQueue.pop();
                }

                // Process data using the function
                if (!data.empty()) {
                    auto output = current.function(data);
                    ans[output.first] = output.second;
                    //debug purpuses
                    // std::cout << "Output: " << output.second << " Answer: " << ans[output.first] << std::endl;
                    // for (int j = 0; j < 4; ++j) {
                    //     std::cout << "ans[" << j << "]: " << ans[j] << std::endl;
                    // }
                    // std::cout<<getResult()<<std::endl; 
                }

                // Pass data to next stage if not the last stage
                if (i < activeObjects.size() - 1) {
                    auto& next = *activeObjects[i + 1];
                    std::unique_lock<std::mutex> lock(next.mutex);
                    next.taskQueue.push(data);
                    next.condition.notify_one();
                }
            } });
    }
}

void Pipeline::stop()
{
    if (!isStarted)
        return;

    // Signal all active objects to stop
    for (auto &ao : activeObjects)
    {
        std::unique_lock<std::mutex> lock(ao->mutex);
        ao->shouldStop = true;
        ao->condition.notify_one();
    }

    // Join all threads
    for (auto &ao : activeObjects)
    {
        if (ao->thread.joinable())
        {
            ao->thread.join();
        }
    }

    isStarted = false;
}

void Pipeline::execute(std::vector<std::pair<int, std::pair<int, int>>> data)
{
    if (!isStarted || activeObjects.empty())
        return;

    // Push data to the first stage
    auto &first = activeObjects[0];
    {
        std::unique_lock<std::mutex> lock(first->mutex);
        first->taskQueue.push(data);
        first->condition.notify_one();
    }
}
std::string Pipeline::getResult()
{
    while (ans[3] == 0)
    {
    } // wait for it to fill up.

    std::string msg = format_msg();
    for (int i = 0; i < 4; i++)
    {
        ans[i] = 0;
    }
    return msg;
}

std::string Pipeline::format_msg()
{
    return "New message \nTotal weight: " + std::to_string(ans[0]) + "\nLongest distance: " +
           std::to_string(ans[1]) + "\nAverage distance: " + std::to_string(ans[2]) +
           "\nShortest MST edge: " + std::to_string(ans[3]) + "\n";
}