// src/server/Server.hpp
#pragma once
#include "../graph/Graph.hpp"
#include "../mst/MSTAlgorithm.hpp"
#include "../utils/Proactor.hpp"
#include <memory>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class Server {
public:
    Server(int port, int threads);
    void start();
    void stop();

private:
    void handleClient(int clientSocket);
    void handleRequest(const std::string& request);
    void addGraph(const Graph& graph);
    void updateGraph(const std::string& changes);
    void solveMST(const std::string& algorithm);

    Graph currentGraph;
    std::unique_ptr<MSTAlgorithm> currentAlgorithm;
    Proactor proactor;
    int serverSocket;
    bool running;
    int port;
};
