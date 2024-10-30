// Pipeline.hpp
#pragma once
#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

class Pipeline
{
public:
    Pipeline(const std::vector<std::function<void(void *)>> &functions);
    Pipeline() = default;
    ~Pipeline();

    void start();                                                   // Start the pipeline
    void stop();                                                    // Stop the pipeline
    void execute(std::vector<std::pair<int, std::pair<int, int>>>); // Execute the pipeline with data

private:
    struct ActiveObject
    {
        ActiveObject(std::function<void(void *)> func)
            : function(func), shouldStop(false) {}

        std::function<void(void *)> function;
        std::thread thread;
        std::mutex mutex;
        std::condition_variable condition;
        std::queue<std::vector<std::pair<int, std::pair<int, int>>>> taskQueue; // Queue to store tasks/data
        bool shouldStop;

        int totalWeight;
        int longestDistance;
        double averageDistance;
        int shortestMSTEdge;
    };

    std::vector<ActiveObject> activeObjects; // Vector of active objects (pipeline stages)
    bool isStarted = false;
};
