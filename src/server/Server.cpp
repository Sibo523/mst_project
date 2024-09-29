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
        return "Graph added successfully.\n";
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
        sendMessage(clientSocket, "Enter MST algorithm (e.g., 'Kruskal', 'Prim', 'IntegerMST', 'Tarjan', 'Boruvka'):");
        std::string algorithm = getClientInput(clientSocket);
        algorithm = trimString(algorithm); // Remove any leading/trailing whitespace
        std::cout << "Algorithm received: '" << algorithm << "'" << std::endl;

        std::vector<std::string> validAlgorithms = {"Kruskal", "Prim", "IntegerMST", "Tarjan", "Boruvka"};
        if (std::find(validAlgorithms.begin(), validAlgorithms.end(), algorithm) != validAlgorithms.end())
        {
            std::string result = solveMST(algorithm);
            std::cout << "MST result: " << result << std::endl;
            return result;
        }
        else
        {
            std::string errorMsg = "Invalid MST algorithm: '" + algorithm + "'. Valid options are: Kruskal, Prim, IntegerMST, Tarjan, Boruvka.\n";
            sendMessage(clientSocket, errorMsg);
            return errorMsg;
        }
    }
    else if (command == "exit")
    {
        stop();
        return "Server stopped.";
    }
    else if (command == "help")
    {
        std::string helpMsg = "Available commands:\n";
        helpMsg += "addGraph: Add a new graph\n";
        helpMsg += "updateGraph: Update the current graph\n";
        helpMsg += "solveMST: Solve the Minimum Spanning Tree problem\n";
        helpMsg += "exit: Stop the server\n";
        return helpMsg;
    }
    else if (command == "clear")
    {
        /* code */
    }

    else
    {
        return "Unknown command: " + command;
    }
}

// Helper function to trim whitespace from a string
std::string trimString(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}
void Server::handleClient(int clientSocket)
{
    char buffer[1024] = {0};
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        sendMessage(clientSocket, "Enter command:\n");
        sendMessage(clientSocket, "1. addGraph\n2. updateGraph\n3. solveMST\n4. exit\n");
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
    std::cout << "Solving MST with algorithm: " << algorithm << std::endl;

    auto mstAlgo = MSTFactory::createMSTAlgorithm(algorithm);
    if (!mstAlgo)
    {
        std::cerr << "Failed to create MST algorithm: " << algorithm << std::endl;
        return "Error: Failed to create MST algorithm.";
    }

    std::vector<std::pair<int, std::pair<int, int>>> mst;
    try
    {
        mst = mstAlgo->findMST(currentGraph);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in findMST: " << e.what() << std::endl;
        return "Error: Exception occurred while finding MST.";
    }

    auto analysis = analyzeMST(currentGraph, mst);

    std::ostringstream oss;
    oss << "MST Analysis using " << algorithm << " algorithm:\n";
    oss << "Total weight: " << analysis.totalWeight << "\n";
    oss << "Longest distance: " << analysis.longestDistance << "\n";
    oss << "Average distance: " << analysis.averageDistance << "\n";
    oss << "Shortest MST edge: " << analysis.shortestMSTEdge << "\n";
    oss << "MST edges: ";
    for (const auto &edge : mst)
    {
        oss << "(" << edge.second.first << "-" << edge.second.second << ", " << edge.first << ") ";
    }
    oss << "\n";

    std::cout << "MST solution: " << oss.str() << std::endl;
    return oss.str();
}