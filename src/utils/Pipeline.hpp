// Pipeline.hpp
#pragma once
#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <sstream>
#include <string>

class Pipeline
{
public:
    Pipeline(const std::vector<std::function<std::vector<std::pair<int, std::pair<int, int>>>>>& functions);
    Pipeline() = default;
    ~Pipeline();

    void start();                                                                     // Start the pipeline
    void stop();                                                                      // Stop the pipeline
    void execute(std::vector<std::pair<int, std::pair<int, int>>>, int clientSocket); // Execute the pipeline with data

private:
    struct ActiveObject
    {
        ActiveObject(std::function<void(std::pair<int, double>)> func)
            : function(func), shouldStop(false) {}

        std::function<void(std::pair<int, double>)> function;
        std::thread thread;
        std::mutex mutex;
        std::condition_variable condition;
        std::queue<std::vector<std::pair<int, std::pair<int, int>>>> taskQueue; // Queue to store tasks/data
        bool shouldStop;
        int clientSocket;
        std::vector<double> ans{0, 0, 0, 0};
    };

    std::string format_msg(ActiveObject &ao);

    std::vector<ActiveObject> activeObjects; // Vector of active objects (pipeline stages)
    bool isStarted = false;
};
