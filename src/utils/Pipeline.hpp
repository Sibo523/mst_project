#pragma once
#include <functional>
#include <array>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <cstddef>
#include <future> //don't know what this is but the compiler told me then this is what I do!

constexpr size_t MAX_PIPELINE_STAGES = 10; // Adjust this value as needed

class Pipeline
{
public:
    Pipeline();
    ~Pipeline();

    void addStage(std::function<void(void *)> stage);

    template <typename T, typename F>
    T process(F inputFunction);

private:
    std::array<std::function<void(void *)>, MAX_PIPELINE_STAGES> stages;
    std::array<std::thread, MAX_PIPELINE_STAGES> threads;
    std::array<std::queue<void *>, MAX_PIPELINE_STAGES> queues;
    std::array<std::mutex, MAX_PIPELINE_STAGES> mutexes;
    std::array<std::condition_variable, MAX_PIPELINE_STAGES> cvs;
    size_t numStages;
    bool running;

    void stageThread(int stageIndex);
};
template <typename T, typename F>
T Pipeline::process(F inputFunction)
{
    std::promise<T> resultPromise;
    std::future<T> resultFuture = resultPromise.get_future();

    // Create a wrapper for the input function
    auto wrappedInput = [inputFunction, &resultPromise]()
    {
        T *result = new T(inputFunction());
        resultPromise.set_value(*result);
        return static_cast<void *>(result);
    };

    // Add the wrapped input function as the first stage
    queues[0].push(wrappedInput());
    cvs[0].notify_one();

    // Wait for the result and clean up
    T result = resultFuture.get();
    delete static_cast<T *>(queues.back().front());
    queues.back().pop();

    return result;
}