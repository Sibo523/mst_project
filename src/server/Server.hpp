#pragma once
#include "../graph/Graph.hpp"
#include "../mst/MSTAlgorithm.hpp"
#include "../utils/Proactor.hpp"
#include <memory>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
public:
    Server(int port, int threads);
    void start();
    void stop();
    std::string handleRequest(const std::string &request);

private:
    void handleClient(int clientSocket);
    void addGraph(const Graph &graph);
    void updateGraph(const std::string &changes);
    std::string solveMST(const std::string &algorithm);

    Graph currentGraph;
    std::unique_ptr<MSTAlgorithm> currentAlgorithm;
    Proactor proactor;
    int serverSocket;
    bool running;
    int port;
};