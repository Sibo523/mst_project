// src/main.cpp
#include "server/Server.hpp"
#include "factory/MSTFactory.hpp"
#include "analysis/MSTAnalysis.hpp"
#include "utils/Benchmark.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void runServer(int port, int threads)
{
    Server server(port, threads);
    server.start();
}

void runBenchmark()
{
    std::cout << "Running benchmark...\n";

    // Create a sample graph
    Graph g(5);
    g.addEdge(0, 1, 2);
    g.addEdge(0, 3, 6);
    g.addEdge(1, 2, 3);
    g.addEdge(1, 3, 8);
    g.addEdge(1, 4, 5);
    g.addEdge(2, 4, 7);
    g.addEdge(3, 4, 9);

    std::vector<std::string> algorithms = {"Boruvka", "Prim", "Kruskal", "Tarjan", "Integer"};

    for (const auto &algo : algorithms)
    {
        auto mstAlgo = MSTFactory::createMSTAlgorithm(algo);

        // Benchmark pipeline approach
        auto pipelineTime = Benchmark::measureTime([&]()
                                                   {
            Pipeline pipeline;
            pipeline.addStage([&](void* data) {
                auto* graph = static_cast<Graph*>(data);
                std::cout<<"graph: "<<graph->getVertices()<<std::endl;
                return mstAlgo->findMST(*graph);
            });
            pipeline.addStage([&](void* data) {
                auto* mst = static_cast<std::vector<std::pair<int, std::pair<int, int>>>*>(data);
                return analyzeMST(g, *mst);
            });
            return pipeline.process<MSTAnalysis>([&]() { return g; }); });

        // Benchmark Leader-Follower approach
        auto lfTime = Benchmark::measureTime([&]()
                                             {
            LeaderFollowerThreadPool pool(4);
            auto future = pool.enqueue([&]() {
                auto mst = mstAlgo->findMST(g);
                return analyzeMST(g, mst);
            });
            return future.get(); });

        std::cout << "Algorithm: " << algo << "\n";
        std::cout << "  Pipeline time: " << pipelineTime << " ms\n";
        std::cout << "  Leader-Follower time: " << lfTime << " ms\n";
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <threads>\n";
        return 1;
    }

    int port = std::stoi(argv[1]);
    int threads = std::stoi(argv[2]);

    std::thread serverThread(runServer, port, threads);

    // Wait a bit for the server to start
    std::this_thread::sleep_for(std::chrono::seconds(1));

    runBenchmark();

    // Keep the server running
    serverThread.join();

    return 0;
}