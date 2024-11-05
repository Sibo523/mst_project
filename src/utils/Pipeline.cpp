
#include "Pipeline.hpp"
Pipeline::Pipeline(const std::vector<std::function<std::pair<int, double>(const std::vector<std::pair<int, std::pair<int, int>>> &)>> &functions)
{
    for (const auto &function : functions)
    {
        activeObjects.push_back(std::make_unique<ActiveObject>(function));
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

    for (size_t i = 0; i < activeObjects.size(); ++i)
    {
        auto &ao = activeObjects[i];

        ao->thread = std::thread([this, i]()
                                 {
            auto& current = *activeObjects[i];
            
            while (true) {
                PipelineData pipelineData;
                {
                    std::unique_lock<std::mutex> lock(current.mutex);
                    current.condition.wait(lock, [&]() {
                        return !current.taskQueue.empty() || current.shouldStop;
                    });

                    if (current.shouldStop && current.taskQueue.empty()) {
                        break;
                    }

                    pipelineData = std::move(current.taskQueue.front());
                    current.taskQueue.pop();
                }

                // Process data using the function and store result
                auto result = current.function(pipelineData.data);
                pipelineData.results.push_back(result);

                // If this is the last stage, store the final result
                if (i == activeObjects.size() - 1) {
                    std::unique_lock<std::mutex> lock(resultMutex);
                    *currentResult = std::move(pipelineData);
                    resultCondition.notify_one();
                }
                // Otherwise, pass to the next stage
                else if (i < activeObjects.size() - 1) {
                    auto& next = *activeObjects[i + 1];
                    std::unique_lock<std::mutex> lock(next.mutex);
                    next.taskQueue.push(std::move(pipelineData));
                    next.condition.notify_one();
                }
            } });
    }
}

void Pipeline::stop()
{
    if (!isStarted)
        return;

    for (auto &ao : activeObjects)
    {
        std::unique_lock<std::mutex> lock(ao->mutex);
        ao->shouldStop = true;
        ao->condition.notify_one();
    }

    for (auto &ao : activeObjects)
    {
        if (ao->thread.joinable())
        {
            ao->thread.join();
        }
    }

    isStarted = false;
}

std::string Pipeline::execute(std::vector<std::pair<int, std::pair<int, int>>> data)
{
    if (!isStarted || activeObjects.empty())
        return "";

    PipelineData pipelineData;
    pipelineData.data = std::move(data);

    // Store pointer to where we want the result
    PipelineData result;
    {
        std::unique_lock<std::mutex> lock(resultMutex);
        currentResult = &result;
    }

    // Push data to the first stage
    {
        auto &first = activeObjects[0];
        std::unique_lock<std::mutex> lock(first->mutex);
        first->taskQueue.push(std::move(pipelineData));
        first->condition.notify_one();
    }

    // Wait for the result
    {
        std::unique_lock<std::mutex> lock(resultMutex);
        resultCondition.wait(lock, [this]()
                             { return currentResult->results.size() == activeObjects.size(); });
    }

    return format_msg(result);
}

std::string Pipeline::format_msg(const PipelineData &data)
{
    std::stringstream ss;
    ss << "New message\n";
    for (const auto &result : data.results)
    {
        switch (result.first)
        {
        case 0:
            ss << "Total weight: " << result.second << "\n";
            break;
        case 1:
            ss << "Longest distance: " << result.second << "\n";
            break;
        case 2:
            ss << "Average distance: " << result.second << "\n";
            break;
        case 3:
            ss << "Shortest MST edge: " << result.second << "\n";
            break;
        }
    }
    return ss.str();
}