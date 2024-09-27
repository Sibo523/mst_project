#include "Server.hpp"
#include "../factory/MSTFactory.hpp"
#include "../analysis/MSTAnalysis.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>

Server::Server(int port, int threads) : currentGraph(0), proactor(threads), port(port), running(false) {}
std::string Server::sendRequest(const std::string& request) {
    return handleRequest(request);
}
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

        proactor.enqueue([this, clientSocket]()
                         { this->handleClient(clientSocket); });
    }
}

void Server::stop()
{
    running = false;
    close(serverSocket);
}

void Server::handleClient(int clientSocket)
{
    char buffer[1024];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(clientSocket, buffer, sizeof(buffer) - 1);
        if (valread <= 0)
        {
            // Client disconnected or error occurred
            break;
        }

        std::string request(buffer);
        std::string response = handleRequest(request);

        send(clientSocket, response.c_str(), response.length(), 0);
    }
    close(clientSocket);
}

std::string Server::handleRequest(const std::string &request)
{
    std::istringstream iss(request);
    std::string command;
    iss >> command;

    std::string response;
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
        response = "Graph added successfully.";
    }
    else if (command == "updateGraph")
    {
        std::string changes;
        std::getline(iss, changes);
        updateGraph(changes);
        response = "Graph updated successfully.";
    }
    else if (command == "solveMST")
    {
        std::string algorithm;
        iss >> algorithm;
        response = solveMST(algorithm);
    }
    else
    {
        response = "Unknown command: " + command;
    }

    std::cout << response << std::endl;
    return response + "\n";
}
std::string Server::sendRequest(const std::string& request) {
    return handleRequest(request);
}

void Server::addGraph(const Graph &graph)
{
    currentGraph = graph;
}

void Server::updateGraph(const std::string &changes)
{
    std::istringstream iss(changes);
    int src, dest, weight;
    while (iss >> src >> dest >> weight)
    {
        currentGraph.addEdge(src, dest, weight);
    }
}

std::string Server::solveMST(const std::string &algorithm)
{
    auto mstAlgo = MSTFactory::createMSTAlgorithm(algorithm);
    auto mst = mstAlgo->findMST(currentGraph);
    auto analysis = analyzeMST(currentGraph, mst);

    std::ostringstream oss;
    oss << "MST Analysis using " << algorithm << " algorithm:\n"
        << "Total weight: " << analysis.totalWeight << "\n"
        << "Longest distance: " << analysis.longestDistance << "\n"
        << "Average distance: " << analysis.averageDistance << "\n"
        << "Shortest MST edge: " << analysis.shortestMSTEdge;

    return oss.str();
}