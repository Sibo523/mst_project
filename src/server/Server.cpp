// src/server/Server.cpp
#include "Server.hpp"
#include "../factory/MSTFactory.hpp"
#include "../analysis/MSTAnalysis.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>

Server::Server(int port, int threads)
    : currentGraph(0),
      threadPool(threads),
      pipeline(),
      port(port),
      running(false) {}

void Server::start()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Failed to create socket\n";
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Failed to bind to port " << port << "\n";
        return;
    }

    if (listen(serverSocket, 3) < 0)
    {
        std::cerr << "Failed to listen on socket\n";
        return;
    }

    std::cout << "Server is listening on port " << port << "\n";

    running = true;
    while (running)
    {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);

        if (clientSocket < 0)
        {
            std::cerr << "Failed to accept client connection\n";
            continue;
        }

        threadPool.enqueue([this, clientSocket]()
                           { this->handleClient(clientSocket); });
    }
}

void Server::stop()
{
    running = false;
    close(serverSocket);
}

std::string Server::handleRequest(const std::string &request)
{
    std::istringstream iss(request);
    std::string command;
    iss >> command;

    if (command == "addGraph")
    {
        int V;
        iss >> V;
        Graph newGraph(V);
        int src, dest, weight;
        while (iss >> src >> dest >> weight)
        {
            newGraph.addEdge(src, dest, weight);
        }
        addGraph(newGraph);
        return "Graph added successfully.";
    }
    else if (command == "updateGraph")
    {
        std::string changes;
        std::getline(iss, changes);
        updateGraph(changes);
        return "Graph updated successfully.";
    }
    else if (command == "solveMST")
    {
        std::string algorithm;
        iss >> algorithm;
        return solveMST(algorithm);
    }
    else
    {
        return "Unknown command: " + command;
    }
}

void Server::handleClient(int clientSocket)
{
    char buffer[1024] = {0};
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(clientSocket, buffer, 1024);
        if (valread <= 0)
        {
            break;
        }
        std::string request(buffer);

        if (request == "exit")
        {
            break;
        }

        std::string response = handleRequest(request);
        send(clientSocket, response.c_str(), response.length(), 0);
    }
    close(clientSocket);
}

void Server::addGraph(const Graph &graph)
{
    currentGraph = graph;
    std::cout << "Graph added successfully." << std::endl;
}

void Server::updateGraph(const std::string &changes)
{
    std::istringstream iss(changes);
    int src, dest, weight;
    while (iss >> src >> dest >> weight)
    {
        currentGraph.addEdge(src, dest, weight);
    }
    std::cout << "Graph updated successfully." << std::endl;
}
std::string Server::solveMST(const std::string &algorithm) {
    auto mstAlgo = MSTFactory::createMSTAlgorithm(algorithm);
    auto mst = mstAlgo->findMST(currentGraph);
    
    auto analysis = pipeline.process<MSTAnalysis>([&]() {
        return analyzeMST(currentGraph, mst);
    });

    std::ostringstream oss;
    oss << "MST Analysis using " << algorithm << " algorithm:\n";
    oss << "Total weight: " << analysis.totalWeight << "\n";
    oss << "Longest distance: " << analysis.longestDistance << "\n";
    oss << "Average distance: " << analysis.averageDistance << "\n";
    oss << "Shortest MST edge: " << analysis.shortestMSTEdge << "\n";

    return oss.str();
}