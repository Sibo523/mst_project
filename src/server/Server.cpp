// src/server/Server.cpp
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
    
    // Send response back to client
    std::string response = "Request processed";
    send(clientSocket, response.c_str(), response.length(), 0);
    close(clientSocket);
}

// Rest of the Server.cpp implementation remains the same
