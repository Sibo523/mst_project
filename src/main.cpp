#include "server/Server.hpp"
#include <iostream>
#include <string>
#include <csignal>
#include <thread>

void runAllFeatures(); // Declaration

Server *serverPtr = nullptr;

void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    if (serverPtr)
    {
        serverPtr->stop();
    }
    exit(signum);
}

int main()
{
    const int PORT = 8081;
    const int THREADS = 4;

    Server server(PORT, THREADS);
    serverPtr = &server;

    // Register signal handler
    signal(SIGINT, signalHandler);

    std::cout << "MST Server starting on port " << PORT << " with " << THREADS << " threads." << std::endl;
    std::cout << "Press Ctrl+C to stop the server." << std::endl;

    // Start the server in a separate thread
    std::thread serverThread([&server]()
                             { server.start(); });

    // Run all features
    // runAllFeatures();

    // Wait for the server thread to finish (which will be when stop() is called)
    serverThread.join();

    return 0;
}

void runAllFeatures()
{
    // This function can now interact with the server using a client-like interface
    // You can implement client-side logic here to test all features of your server
    std::cout << "Running all features..." << std::endl;

    // Example: Add a graph
    std::string addGraphRequest = "addGraph 4 0 1 10 0 2 6 0 3 5 1 3 15 2 3 4";
    std::string response = serverPtr->handleRequest(addGraphRequest);
    std::cout << "Add Graph Response: " << response << std::endl;

    // Example: Solve MST using different algorithms
    std::string solvePrimRequest = "solveMST Prim";
    response = serverPtr->handleRequest(solvePrimRequest);
    std::cout << "Solve MST (Prim) Response:\n"
              << response << std::endl;

    std::string solveKruskalRequest = "solveMST Kruskal";
    response = serverPtr->handleRequest(solveKruskalRequest);
    std::cout << "Solve MST (Kruskal) Response:\n"
              << response << std::endl;

    // Add more feature tests as needed

    std::cout << "All features tested." << std::endl;
}