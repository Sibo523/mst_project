// // tests/AnalysisTests.cpp
// #include "../src/analysis/MSTAnalysis.hpp"
// #include "../src/graph/Graph.hpp"
// #include <cassert>
// #include <iostream>
// #include <cmath>

// // Helper function to create a test graph and MST
// std::pair<Graph, std::vector<std::pair<int, std::pair<int, int>>>> createTestGraphAndMST()
// {
//     Graph g(4);
//     g.addEdge(0, 1, 10);
//     g.addEdge(0, 2, 6);
//     g.addEdge(0, 3, 5);
//     g.addEdge(1, 3, 15);
//     g.addEdge(2, 3, 4);

//     std::vector<std::pair<int, std::pair<int, int>>> mst = {
//         {5, {0, 3}},
//         {4, {2, 3}},
//         {6, {0, 2}}};

//     return {g, mst};
// }

// void testCalculateTotalWeight()
// {
//     auto [g, mst] = createTestGraphAndMST();
//     int totalWeight = calculateTotalWeight(mst);
//     assert(totalWeight == 15);
//     std::cout << "Calculate total weight test passed.\n";
// }

// void testFindLongestDistance()
// {
//     auto [g, mst] = createTestGraphAndMST();
//     int longestDistance = findLongestDistance(g, mst);
//     assert(longestDistance == 11); // Path: 1 -> 0 -> 2 -> 3
//     std::cout << "Find longest distance test passed.\n";
// }

// void testCalculateAverageDistance()
// {
//     auto [g, mst] = createTestGraphAndMST();
//     double avgDistance = calculateAverageDistance(g, mst);
//     // Expected average: (5 + 6 + 11 + 5 + 4 + 9) / 6 = 6.6666...
//     assert(std::abs(avgDistance - 6.6666667) < 0.0001);
//     std::cout << "Calculate average distance test passed.\n";
// }

// void testFindShortestMSTEdge()
// {
//     auto [g, mst] = createTestGraphAndMST();
//     int shortestEdge = findShortestMSTEdge(mst);
//     assert(shortestEdge == 4);
//     std::cout << "Find shortest MST edge test passed.\n";
// }

// void testAnalyzeMST()
// {
//     auto [g, mst] = createTestGraphAndMST();
//     MSTAnalysis analysis = analyzeMST(g, mst);

//     assert(analysis.totalWeight == 15);
//     assert(analysis.longestDistance == 11);
//     assert(std::abs(analysis.averageDistance - 6.6666667) < 0.0001);
//     assert(analysis.shortestMSTEdge == 4);

//     std::cout << "Analyze MST test passed.\n";
// }

// // int main() {
// //     testCalculateTotalWeight();
// //     testFindLongestDistance();
// //     testCalculateAverageDistance();
// //     testFindShortestMSTEdge();
// //     testAnalyzeMST();

// //     std::cout << "All Analysis tests passed!\n";
// //     return 0;
// // }
