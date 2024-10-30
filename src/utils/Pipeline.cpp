Pipeline::Pipeline(const std::vector<std::function<void(void *)>> &functions)
{
    // Create active objects for each function
    for (const auto &function : functions)
    {
        activeObjects.emplace_back(function);
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

    // Start a thread for each active object
    for (size_t i = 0; i < activeObjects.size(); ++i)
    {
        auto &ao = activeObjects[i];

        ao.thread = std::thread([this, i]()
                                {
            auto& current = activeObjects[i];
            
            while (true) {
                void* data = nullptr;
                
                // Wait for and get data from queue
                {
                    std::unique_lock<std::mutex> lock(current.mutex);
                    current.condition.wait(lock, [&]() {
                        return !current.taskQueue.empty() || current.shouldStop;
                    });

                    if (current.shouldStop && current.taskQueue.empty()) {
                        break;
                    }

                    data = current.taskQueue.front();
                    current.taskQueue.pop();
                }

                // Process data run the function on the mst
                if (data) {
                    current.function(data);
                }

                // Pass data to next stage if not the last stage
                if (i < activeObjects.size() - 1) {
                    auto& next = activeObjects[i + 1];
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
        std::unique_lock<std::mutex> lock(ao.mutex);
        ao.shouldStop = true;
        ao.condition.notify_one();
    }

    // Join all threads
    for (auto &ao : activeObjects)
    {
        if (ao.thread.joinable())
        {
            ao.thread.join();
        }
    }

    isStarted = false;
}
// the graph I think
void Pipeline::execute(std::vector<std::pair<int, std::pair<int, int>>> data)
{
    if (!isStarted || activeObjects.empty())
        return;

    // Push data to the first stage
    auto &first = activeObjects[0];
    {
        std::unique_lock<std::mutex> lock(first.mutex);
        first.taskQueue.push(data);
        first.condition.notify_one();
    }
}