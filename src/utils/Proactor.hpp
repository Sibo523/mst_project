#ifndef PROACTOR_HPP
#define PROACTOR_HPP

#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>

class Proactor;

using Handler = std::function<void(int)>;

class Proactor
{
public:
    Proactor();
    ~Proactor();

    void stop();
    void post(int clientSocket, Handler handler);
    void run(int fd, Handler handler);
    std::thread* get_thread();

private:
    void workerThread();

    std::queue<std::pair<int, Handler>> handlers;
    std::mutex handlersMutex;
    std::condition_variable handlersCondition;
    std::atomic<bool> running;
    std::vector<std::thread> workers;
    int fd;
    Handler handler;
    std::thread *eventLoopThread;
};

#endif // PROACTOR_HPP