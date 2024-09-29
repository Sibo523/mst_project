#include "server/Server.hpp"
#include "factory/MSTFactory.hpp"
#include "analysis/MSTAnalysis.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void runServer(int port, int threads)
{
    Server server(port, threads);
    server.start();
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <threads>\n";
        return 1;
    }

    int port = std::stoi(argv[1]);
    int threads = std::stoi(argv[2]);

    std::thread serverThread(runServer, port, threads);

    // Wait a bit for the server to start
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Run MST algorithm test
    std::cout << "\nServer is running. Press Ctrl+C to stop.\n";

    // Keep the server running
    serverThread.join();

    return 0;
}