#pragma once
#include "../graph/Graph.hpp"
#include "../mst/MSTAlgorithm.hpp"
#include "../utils/LeaderFollowerThreadPool.hpp"
#include "../utils/Pipeline.hpp"
#include "../utils/Proactor.hpp"
#include "../analysis/MSTAnalysis.hpp"
#include <memory>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <future>
#include <type_traits>
#include <utility>

class Server
{
public:
    Server(int port, int threads);
    void start();
    void stop();
    std::string handleRequest(const std::string &request, int socket);

private:
    void handleClient(int clientSocket);
    void addGraph(const Graph &graph);
    void updateGraph(const std::string &changes);
    std::string solveMST(const std::string &algorithm, int choice, int clientSocket);
    void sendMessage(int clientSocket, const std::string &message);
    std::string getClientInput(int clientSocket);
    std::mutex graphMutex;
    Graph currentGraph;
    std::unique_ptr<MSTAlgorithm> currentAlgorithm;
    LeaderFollowerThreadPool threadPool;
    Pipeline pipeline;
    Proactor proactor; // Replace troll with proactor
    int serverSocket;
    bool running;
    int port;
};

std::string trimString(const std::string &str);