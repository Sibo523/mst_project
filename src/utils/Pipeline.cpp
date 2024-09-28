// src/utils/Pipeline.cpp
#include "Pipeline.hpp"
#include <stdexcept>
#include <cstddef> // Add this line to include the <cstddef> header
#include <thread>

Pipeline::Pipeline() : numStages(0), running(true) {}

Pipeline::~Pipeline()
{
    running = false;
    for (std::size_t i = 0; i < numStages; ++i)
    { // Replace 'size_t' with 'std::size_t'
        cvs[i].notify_all();
    }
    for (std::size_t i = 0; i < numStages; ++i)
    { // Replace 'size_t' with 'std::size_t'
        if (threads[i].joinable())
        {
            threads[i].join();
        }
    }
}
void Pipeline::addStage(std::function<void(void *)> stage)
{
    if (numStages >= MAX_PIPELINE_STAGES)
    {
        throw std::runtime_error("Maximum number of pipeline stages reached");
    }
    stages[numStages] = stage;
    threads[numStages] = std::thread([this, stageIndex = static_cast<int>(numStages)]()
                                     { this->stageThread(stageIndex); });
    ++numStages;
}
void Pipeline::stageThread(int stageIndex)
{
    while (running)
    {
        std::unique_lock<std::mutex> lock(mutexes[stageIndex]);
        cvs[stageIndex].wait(lock, [this, stageIndex]
                             { return !running || !queues[stageIndex].empty(); });

        if (!running && queues[stageIndex].empty())
        {
            break;
        }

        void *data = queues[stageIndex].front();
        queues[stageIndex].pop();
        lock.unlock();

        stages[stageIndex](data);

        if (stageIndex < numStages - 1)
        {
            std::lock_guard<std::mutex> nextLock(mutexes[stageIndex + 1]);
            queues[stageIndex + 1].push(data);
            cvs[stageIndex + 1].notify_one();
        }
    }
}