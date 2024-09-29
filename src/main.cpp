#include "server/Server.hpp"
#include "factory/MSTFactory.hpp"
#include "analysis/MSTAnalysis.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void runServer(int port, int threads)
{
    Server server(port, threads);
    server.start();
}


// void testMSTAlgorithms()
//{
//  std::cout << "Testing MST Algorithms...\n";

// // Create a sample graph
// Graph g(5);
// g.addEdge(0, 1, 2);
// g.addEdge(0, 3, 6);
// g.addEdge(1, 2, 3);
// g.addEdge(1, 3, 8);
// g.addEdge(1, 4, 5);
// g.addEdge(2, 4, 7);
// g.addEdge(3, 4, 9);

// std::cout << "Graph created with 5 vertices and 7 edges.\n";

// std::vector<std::string> algorithms = {"Boruvka", "Prim", "Kruskal", "Tarjan"};

// for (const auto &algo : algorithms)
// {
//     std::cout << "\nTesting " << algo << " algorithm:\n";

//     auto mstAlgo = MSTFactory::createMSTAlgorithm(algo);
//     if (mstAlgo == nullptr)
//     {
//         std::cerr << "Error: Failed to create " << algo << " algorithm.\n";
//         continue;
//     }

//     try
//     {
//         auto mst = mstAlgo->findMST(g);
//         auto analysis = server.analyzeMST(g, mst);

//         std::cout << "MST Analysis:\n";
//         std::cout << "  Total weight: " << analysis.totalWeight << "\n";
//         std::cout << "  Longest distance: " << analysis.longestDistance << "\n";
//         std::cout << "  Average distance: " << analysis.averageDistance << "\n";
//         std::cout << "  Shortest MST edge: " << analysis.shortestMSTEdge << "\n";

//         std::cout << "  MST edges: ";
//         for (const auto &edge : mst)
//         {
//             std::cout << "(" << edge.second.first << "-" << edge.second.second << ", " << edge.first << ") ";
//         }
//         std::cout << "\n";
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Error running " << algo << " algorithm: " << e.what() << "\n";
//     }
// }
//}

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

    // Run MST algorithm tests
   // testMSTAlgorithms();

    std::cout << "\nServer is running. Press Ctrl+C to stop.\n";

    // Keep the server running
    serverThread.join();

    return 0;
}