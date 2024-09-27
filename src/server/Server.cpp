#include "Server.hpp"
#include "../factory/MSTFactory.hpp"
#include "../analysis/MSTAnalysis.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>

Server::Server(int port, int threads) : currentGraph(0), proactor(threads), port(port), running(false) {}

void Server::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket\n";
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to bind to port " << port << "\n";
        return;
    }

    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Failed to listen on socket\n";
        return;
    }

    std::cout << "Server is listening on port " << port << "\n";

    running = true;
    while (running) {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        
        if (clientSocket < 0) {
            std::cerr << "Failed to accept client connection\n";
            continue;
        }

        proactor.enqueue([this, clientSocket]() {
            this->handleClient(clientSocket);
        });
    }
}

void Server::stop() {
    running = false;
    close(serverSocket);
}

void Server::handleClient(int clientSocket) {
    char buffer[1024] = {0};
    int valread = read(clientSocket, buffer, 1024);
    std::string request(buffer);
    
    handleRequest(request);
    
    std::string response = "Request processed";
    send(clientSocket, response.c_str(), response.length(), 0);
    close(clientSocket);
}

void Server::handleRequest(const std::string& request) {
    std::istringstream iss(request);
    std::string command;
    iss >> command;

    if (command == "addGraph") {
        int V;
        iss >> V;
        Graph newGraph(V);
        int src, dest, weight;
        while (iss >> src >> dest >> weight) {
            newGraph.addEdge(src, dest, weight);
        }
        addGraph(newGraph);
    } else if (command == "updateGraph") {
        std::string changes;
        std::getline(iss, changes);
        updateGraph(changes);
    } else if (command == "solveMST") {
        std::string algorithm;
        iss >> algorithm;
        solveMST(algorithm);
    } else {
        std::cout << "Unknown command: " << command << std::endl;
    }
}

void Server::addGraph(const Graph& graph) {
    currentGraph = graph;
    std::cout << "Graph added successfully." << std::endl;
}

void Server::updateGraph(const std::string& changes) {
    std::istringstream iss(changes);
    int src, dest, weight;
    while (iss >> src >> dest >> weight) {
        currentGraph.addEdge(src, dest, weight);
    }
    std::cout << "Graph updated successfully." << std::endl;
}

void Server::solveMST(const std::string& algorithm) {
    auto mstAlgo = MSTFactory::createMSTAlgorithm(algorithm);
    auto mst = mstAlgo->findMST(currentGraph);
    auto analysis = analyzeMST(currentGraph, mst);
    
    std::cout << "MST Analysis using " << algorithm << " algorithm:" << std::endl;
    std::cout << "Total weight: " << analysis.totalWeight << std::endl;
    std::cout << "Longest distance: " << analysis.longestDistance << std::endl;
    std::cout << "Average distance: " << analysis.averageDistance << std::endl;
    std::cout << "Shortest MST edge: " << analysis.shortestMSTEdge << std::endl;
}