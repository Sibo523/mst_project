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

class Pipeline {
public:
    Pipeline(const std::vector<std::function<std::pair<int, double>(const std::vector<std::pair<int, std::pair<int, int>>>&)>>& functions);
    Pipeline() = default;
    ~Pipeline();

    void start();
    void stop();
    std::string execute(std::vector<std::pair<int, std::pair<int, int>>> data);

private:
    struct PipelineData {
        std::vector<std::pair<int, std::pair<int, int>>> data;
        std::vector<std::pair<int, double>> results;
    };

    class ActiveObject {
    public:
        ActiveObject(std::function<std::pair<int, double>(const std::vector<std::pair<int, std::pair<int, int>>>&)> func)
            : function(std::move(func)), shouldStop(false) {}

        ActiveObject(const ActiveObject&) = delete;
        ActiveObject& operator=(const ActiveObject&) = delete;
        ActiveObject(ActiveObject&&) = default;
        ActiveObject& operator=(ActiveObject&&) = default;

        std::function<std::pair<int, double>(const std::vector<std::pair<int, std::pair<int, int>>>&)> function;
        std::thread thread;
        std::mutex mutex;
        std::condition_variable condition;
        std::queue<PipelineData> taskQueue;
        bool shouldStop;
    };

    std::string format_msg(const PipelineData& data);
    std::vector<std::unique_ptr<ActiveObject>> activeObjects;
    bool isStarted = false;
    std::mutex resultMutex;
    std::condition_variable resultCondition;
    PipelineData* currentResult = nullptr;
};
