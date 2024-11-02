#include "Proactor.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

Proactor::Proactor() : running(true)
{
    // We'll initialize the worker thread in the run method
}

Proactor::~Proactor()
{
    stop();
    if (eventLoopThread && eventLoopThread->joinable())
    {
        eventLoopThread->join();
        delete eventLoopThread;
    }
    std::cout << "Proactor stopped and worker thread joined." << std::endl;
}

void Proactor::stop()
{
    {
        std::unique_lock<std::mutex> lock(handlersMutex);
        running = false;
    }
    handlersCondition.notify_all();
    std::cout << "Stop signal sent to worker thread." << std::endl;
}

void Proactor::post(int clientSocket, Handler handler)
{
    {
        std::unique_lock<std::mutex> lock(handlersMutex);
        handlers.emplace(clientSocket, handler);
    }
    // std::thread(&handler, clientSocket).detach();
    handlersCondition.notify_one();
    std::cout << "New task posted for client socket: " << clientSocket << std::endl;
}

void Proactor::run(int fd, Handler handler)
{
    this->fd = fd;
    this->handler = handler;
    {
        std::unique_lock<std::mutex> lock(handlersMutex);
        running = true;
    }
    this->eventLoopThread = new std::thread(&Proactor::workerThread, this);
}

void Proactor::workerThread()
{
    std::cout << "Worker thread started." << std::endl;

    bool run = true;

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    while (run)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        int s = select(fd + 1, &readfds, NULL, NULL, &tv);
        if (s > 0)
        {
            int clientSocket = accept(fd, NULL, NULL);
            if (clientSocket < 0)
            {
                std::cout << "Accept failed." << std::endl;
                break;
            }
            std::cout << "Accepted new client socket: " << clientSocket << std::endl;
            std::thread(handler, clientSocket).detach();
        }
        if (s < 0)
        {
            std::cout << "Select failed." << std::endl;
            break;
        }
        else if (s == 0)
        {
            continue;
        }
        {
            std::lock_guard<std::mutex> guard(handlersMutex);
            run = running; // Update the still_running variable
        }
    }
}

std::thread *Proactor::get_thread()
{
    return this->eventLoopThread;
}
