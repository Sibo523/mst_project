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
#include <memory>

class Pipeline
{
public:
    Pipeline(const std::vector<std::function<std::pair<int, double>(const std::vector<std::pair<int, std::pair<int, int>>> &)>> &functions);
    Pipeline() = default;
    ~Pipeline();

    void start();
    void stop();
    void execute(std::vector<std::pair<int, std::pair<int, int>>> data);
    std::string getResult();

private:
    class ActiveObject
    {
        public:
            ActiveObject(std::function<std::pair<int, double>(const std::vector<std::pair<int, std::pair<int, int>>> &)> func)
                : function(std::move(func)), shouldStop(false) {}

            // Delete copy constructor and assignment operator
            ActiveObject(const ActiveObject &) = delete;
            ActiveObject &operator=(const ActiveObject &) = delete;

            // Add move constructor and assignment operator
            ActiveObject(ActiveObject &&) = default;
            ActiveObject &operator=(ActiveObject &&) = default;

            std::function<std::pair<int, double>(const std::vector<std::pair<int, std::pair<int, int>>> &)> function;
            std::thread thread;
            std::mutex mutex;
            std::condition_variable condition;
            std::queue<std::vector<std::pair<int, std::pair<int, int>>>> taskQueue;
            bool shouldStop;
    };
    
    std::vector<double> ans;
    std::string format_msg();
    std::vector<std::unique_ptr<ActiveObject>> activeObjects;
    bool isStarted = false;
};
;