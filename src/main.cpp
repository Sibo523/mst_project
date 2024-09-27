#include "server/Server.hpp"
#include <iostream>
#include <string>
#include <csignal>

void runAllFeatures();  // Declaration

Server* serverPtr = nullptr;

void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    if (serverPtr) {
        serverPtr->stop();
    }
    exit(signum);
}

int main() {
    const int PORT = 8080;
    const int THREADS = 4;

    Server server(PORT, THREADS);
    serverPtr = &server;

    // Register signal handler
    signal(SIGINT, signalHandler);

    std::cout << "MST Server starting on port " << PORT << " with " << THREADS << " threads." << std::endl;
    std::cout << "Press Ctrl+C to stop the server." << std::endl;

    server.start();

    // Run all features
    runAllFeatures();

    return 0;
}