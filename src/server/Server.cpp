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

std::string Server::handleRequest(const std::string &request, int clientSocket)
{
    std::istringstream iss(request);
    std::string command;
    iss >> command;

    if (command == "addGraph")
    {
        sendMessage(clientSocket, "Enter the number of vertices:");
        std::string input = getClientInput(clientSocket);
        int V = std::stoi(input);
        Graph newGraph(V);

        while (true)
        {
            sendMessage(clientSocket, "Enter edges (src dest weight), '-1' when finished:\n");
            input = getClientInput(clientSocket);
            if (input == "-1" || input == "-1\n" || input == "-1\r\n")
                break;

            std::istringstream edgeStream(input);
            int src, dest, weight;
            if (edgeStream >> src >> dest >> weight)
            {
                try
                {
                    newGraph.addEdge(src, dest, weight);
                    sendMessage(clientSocket, "Edge added successfully. src: " + std::to_string(src) + ", dest: " + std::to_string(dest) + ", weight: " + std::to_string(weight) + "\n");
                }
                catch (const std::exception &e)
                {
                    sendMessage(clientSocket, e.what() + '\n');
                }
            }
            else
            {
                sendMessage(clientSocket, "Invalid input. Try again or type 'done':\n");
            }
        }

        addGraph(newGraph);
        return "Graph added successfully.";
    }
    else if (command == "updateGraph")
    {
        sendMessage(clientSocket, "Enter graph changes:");
        std::string changes = getClientInput(clientSocket);
        updateGraph(changes);
        return "Graph updated successfully.";
    }
    else if (command == "solveMST")
    {
        sendMessage(clientSocket, "Enter MST algorithm (e.g., 'Kruskal' or 'Prim','IntegerMST','Tarjan','Boruvka'):");
        std::string algorithm = getClientInput(clientSocket);
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

        std::string response = handleRequest(request, clientSocket);
        sendMessage(clientSocket, response);
    }
    close(clientSocket);
}
void Server::sendMessage(int clientSocket, const std::string &message)
{
    send(clientSocket, message.c_str(), message.length(), 0);
}

std::string Server::getClientInput(int clientSocket)
{
    char buffer[1024] = {0};
    int valread = read(clientSocket, buffer, 1024);
    if (valread <= 0)
    {
        return "";
    }
    return std::string(buffer);
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
std::string Server::solveMST(const std::string &algorithm)
{
    auto mstAlgo = MSTFactory::createMSTAlgorithm(algorithm);
    
    auto mst = mstAlgo->findMST(currentGraph);

    auto analysis = pipeline.process<MSTAnalysis>([&]()
                                                  { return analyzeMST(currentGraph, mst); });

    std::ostringstream oss;
    oss << "MST Analysis using " << algorithm << " algorithm:\n";
    oss << "Total weight: " << analysis.totalWeight << "\n";
    oss << "Longest distance: " << analysis.longestDistance << "\n";
    oss << "Average distance: " << analysis.averageDistance << "\n";
    oss << "Shortest MST edge: " << analysis.shortestMSTEdge << "\n";

    return oss.str();
}