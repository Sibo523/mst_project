// src/server/Server.hpp
#pragma once
#include "../graph/Graph.hpp"
#include "../mst/MSTAlgorithm.hpp"
#include "../utils/ActiveObject.hpp"
#include "../utils/LeaderFollowerThreadPool.hpp"
#include "../utils/Pipeline.hpp"
#include <memory>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class Server : public ActiveObject {
public:
    Server(int port, int threads);
    void start();
    void stop();
    std::string handleRequest(const std::string &request) override;

private:
    void handleClient(int clientSocket);
    void addGraph(const Graph &graph);
    void updateGraph(const std::string &changes);
    std::string solveMST(const std::string &algorithm);

    Graph currentGraph;
    std::unique_ptr<MSTAlgorithm> currentAlgorithm;
    LeaderFollowerThreadPool threadPool;
    Pipeline pipeline;
    int serverSocket;
    bool running;
    int port;
};