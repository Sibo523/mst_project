#pragma once
#include <functional>
#include <array>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>
#include <future>

constexpr size_t MAX_PIPELINE_STAGES = 10;

class Pipeline
{
public:
    Pipeline();
    ~Pipeline();

    void addStage(std::function<void(std::shared_ptr<void>)> stage);

    template <typename T, typename F>
    T process(F inputFunction);

private:
    std::array<std::function<void(std::shared_ptr<void>)>, MAX_PIPELINE_STAGES> stages;
    std::array<std::thread, MAX_PIPELINE_STAGES> threads;
    std::array<std::queue<std::shared_ptr<void>>, MAX_PIPELINE_STAGES> queues;
    std::array<std::mutex, MAX_PIPELINE_STAGES> mutexes;
    std::array<std::condition_variable, MAX_PIPELINE_STAGES> cvs;
    size_t numStages;
    bool running;

    void stageThread(int stageIndex);
};

template <typename T, typename F>
T Pipeline::process(F inputFunction)
{
    auto input = std::make_shared<T>(inputFunction());
    
    // Push the input into the first stage
    {
        std::lock_guard<std::mutex> lock(mutexes[0]);
        queues[0].push(std::static_pointer_cast<void>(input));
    }
    cvs[0].notify_one();

    // Wait for the result from the last stage
    std::shared_ptr<T> result;
    {
        std::unique_lock<std::mutex> lock(mutexes[numStages - 1]);
        cvs[numStages - 1].wait(lock, [this] { return !queues[numStages - 1].empty(); });
        result = std::static_pointer_cast<T>(queues[numStages - 1].front());
        queues[numStages - 1].pop();
    }

    return *result;
}