// src/utils/ActiveObject.cpp
#include "ActiveObject.hpp"

ActiveObject::ActiveObject() : stop_flag(false) {
    worker = std::thread(&ActiveObject::processQueue, this);
}

ActiveObject::~ActiveObject() {
    stop();
    if (worker.joinable()) {
        worker.join();
    }
}

void ActiveObject::run() {
    stop_flag = false;
    if (!worker.joinable()) {
        worker = std::thread(&ActiveObject::processQueue, this);
    }
}

void ActiveObject::stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop_flag = true;
    }
    condition.notify_all();
}

void ActiveObject::processQueue() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this] { return stop_flag || !tasks.empty(); });
            if (stop_flag && tasks.empty()) {
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}