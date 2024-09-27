#include "server/Server.hpp"
#include "graph/Graph.hpp"
#include <iostream>
#include <vector>
#include <utility>
#include <chrono>
#include <thread>

void runAllFeatures() {
    const int PORT = 8080;
    const int THREADS = 4;
    Server server(PORT, THREADS);

    // Create a graph
    Graph graph(5);
    std::vector<std::tuple<int, int, int>> edges = {
        {0, 1, 2}, {0, 2, 3}, {1, 2, 1}, {1, 3, 4},
        {2, 3, 5}, {2, 4, 6}, {3, 4, 7}
    };
    for (const auto& [src, dest, weight] : edges) {
        graph.addEdge(src, dest, weight);
    }

    // Send requests to the server
    server.handleRequest("addGraph 5 0 1 2 0 2 3 1 2 1 1 3 4 2 3 5 2 4 6 3 4 7");
    server.handleRequest("updateGraph 1 3 8");

    // Solve MST using different algorithms
    std::vector<std::string> algorithms = {"Prim", "Kruskal", "Boruvka", "Tarjan"};
    for (const auto& algo : algorithms) {
        server.handleRequest("solveMST " + algo);
    }

    // Wait for all tasks to complete
    std::this_thread::sleep_for(std::chrono::seconds(2));
}