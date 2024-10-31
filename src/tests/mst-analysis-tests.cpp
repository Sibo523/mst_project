// #include <gtest/gtest.h>
// #include "analysis/MSTAnalysis.hpp"
// #include "graph/Graph.hpp"
// #include <vector>
// #include <future>
// #include <chrono>

// class MSTAnalysisTest : public ::testing::Test
// {
// protected:
//     Graph graph{5}; // Create a graph with 5 vertices
//     std::vector<std::pair<int, std::pair<int, int>>> mst;

//     void SetUp() override
//     {
//         // Set up a sample MST
//         graph.addEdge(0, 1, 2);
//         graph.addEdge(1, 2, 3);
//         graph.addEdge(2, 3, 4);
//         graph.addEdge(3, 4, 5);

//         mst = {
//             {2, {0, 1}},
//             {3, {1, 2}},
//             {4, {2, 3}},
//             {5, {3, 4}}};
//     }
// };

// TEST_F(MSTAnalysisTest, CalculateTotalWeightAsync)
// {
//     auto future = calculateTotalWeightAsync(mst);
//     EXPECT_EQ(future.get(), 14); // 2 + 3 + 4 + 5 = 14
// }

// TEST_F(MSTAnalysisTest, FindLongestDistanceAsync)
// {
//     auto future = findLongestDistanceAsync(graph, mst);
//     EXPECT_EQ(future.get(), 14); // Longest path: 0 -> 1 -> 2 -> 3 -> 4
// }

// TEST_F(MSTAnalysisTest, CalculateAverageDistanceAsync)
// {
//     auto future = calculateAverageDistanceAsync(graph, mst);
//     double expected = (2.0 + 5.0 + 9.0 + 14.0) / 4.0; // Average of all possible paths
//     EXPECT_NEAR(future.get(), expected, 0.5);
// }

// TEST_F(MSTAnalysisTest, FindShortestMSTEdgeAsync)
// {
//     auto future = findShortestMSTEdgeAsync(mst);
//     EXPECT_EQ(future.get(), 2); // The shortest edge in the MST
// }

// TEST_F(MSTAnalysisTest, EmptyMST)
// {
//     std::vector<std::pair<int, std::pair<int, int>>> emptyMST;

//     auto totalWeightFuture = calculateTotalWeightAsync(emptyMST);
//     EXPECT_EQ(totalWeightFuture.get(), 0);

//     auto shortestEdgeFuture = findShortestMSTEdgeAsync(emptyMST);
//     EXPECT_EQ(shortestEdgeFuture.get(), 0);
// }

// TEST_F(MSTAnalysisTest, ConcurrentExecution)
// {
//     auto start = std::chrono::high_resolution_clock::now();

//     auto totalWeightFuture = calculateTotalWeightAsync(mst);
//     auto longestDistanceFuture = findLongestDistanceAsync(graph, mst);
//     auto averageDistanceFuture = calculateAverageDistanceAsync(graph, mst);
//     auto shortestEdgeFuture = findShortestMSTEdgeAsync(mst);

//     totalWeightFuture.get();
//     longestDistanceFuture.get();
//     averageDistanceFuture.get();
//     shortestEdgeFuture.get();

//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

//     // This test assumes that concurrent execution should be faster than sequential
//     // The exact time will depend on the machine, but it should be significantly less than the sum of individual operations
//     EXPECT_LT(duration.count(), 1000); // Adjust this value based on your system's performance
// }

// TEST_F(MSTAnalysisTest, NegativeWeights)
// {
//     mst.push_back({-1, {4, 0}}); // Add a negative weight edge

//     auto totalWeightFuture = calculateTotalWeightAsync(mst);
//     EXPECT_EQ(totalWeightFuture.get(), 14); // The negative weight should be ignored

//     auto shortestEdgeFuture = findShortestMSTEdgeAsync(mst);
//     EXPECT_EQ(shortestEdgeFuture.get(), -1); // The negative weight should not affect the shortest edge
// }
