// src/utils/ActiveObject.hpp
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <future>

class ActiveObject
{
public:
    ActiveObject();
    virtual ~ActiveObject();

    template <class F, class... Args>
    auto enqueue(F &&f, Args &&...args)
        -> std::future<typename std::result_of<F(Args...)>::type>;

    virtual std::string handleRequest(const std::string &request, int socket) = 0;

protected:
    void run();
    void stop();

private:
    std::thread worker;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop_flag;

    void processQueue();
};

template <class F, class... Args>
auto ActiveObject::enqueue(F &&f, Args &&...args)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if (stop_flag)
            throw std::runtime_error("enqueue on stopped ActiveObject");
        tasks.emplace([task]()
                      { (*task)(); });
    }
    condition.notify_one();
    return res;
}