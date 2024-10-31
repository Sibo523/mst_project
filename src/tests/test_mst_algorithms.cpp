// // test_mst_algorithms.cpp
// #include <gtest/gtest.h>
// #include "graph/Graph.hpp"
// #include "factory/MSTFactory.hpp"
// #include "analysis/MSTAnalysis.hpp"
// #include <memory>
// #include <vector>
// #include <string>
// #include <future>
// #include <algorithm>

// class MSTTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//         // Create a sample graph
//         g = std::make_unique<Graph>(5);
//         g->addEdge(0, 1, 2);
//         g->addEdge(0, 3, 6);
//         g->addEdge(1, 2, 3);
//         g->addEdge(1, 3, 8);
//         g->addEdge(1, 4, 5);
//         g->addEdge(2, 4, 7);
//         g->addEdge(3, 4, 9);
//     }

//     std::unique_ptr<Graph> g;
//     std::vector<std::string> algorithms = {"Boruvka", "Prim", "Kruskal", "Tarjan"};
// };

// TEST_F(MSTTest, CorrectTotalWeight)
// {
//     for (const auto &algo : algorithms)
//     {
//         auto mstAlgo = MSTFactory::createMSTAlgorithm(algo);
//         ASSERT_NE(mstAlgo, nullptr) << "Failed to create " << algo << " algorithm";

//         auto mst = mstAlgo->findMST(*g);
//         auto totalWeightFuture = calculateTotalWeightAsync(mst);
//         EXPECT_EQ(totalWeightFuture.get(), 16) << "Incorrect total weight for " << algo << " algorithm";
//     }
// }

// TEST_F(MSTTest, CorrectEdgeCount)
// {
//     for (const auto &algo : algorithms)
//     {
//         auto mstAlgo = MSTFactory::createMSTAlgorithm(algo);
//         ASSERT_NE(mstAlgo, nullptr) << "Failed to create " << algo << " algorithm";

//         auto mst = mstAlgo->findMST(*g);

//         EXPECT_EQ(mst.size(), g->getVertices() - 1) << "Incorrect number of edges in MST for " << algo << " algorithm";
//     }
// }

// TEST_F(MSTTest, ConnectedMST)
// {
//     for (const auto &algo : algorithms)
//     {
//         auto mstAlgo = MSTFactory::createMSTAlgorithm(algo);
//         ASSERT_NE(mstAlgo, nullptr) << "Failed to create " << algo << " algorithm";

//         auto mst = mstAlgo->findMST(*g);

//         // Check if MST is connected (simplified check)
//         std::vector<bool> visited(g->getVertices(), false);
//         std::function<void(int)> dfs = [&](int v)
//         {
//             visited[v] = true;
//             for (const auto &edge : mst)
//             {
//                 if (edge.second.first == v && !visited[edge.second.second])
//                 {
//                     dfs(edge.second.second);
//                 }
//                 else if (edge.second.second == v && !visited[edge.second.first])
//                 {
//                     dfs(edge.second.first);
//                 }
//             }
//         };
//         dfs(0);

//         bool all_visited = std::all_of(visited.begin(), visited.end(), [](bool v)
//                                        { return v; });
//         EXPECT_TRUE(all_visited) << "MST is not connected for " << algo << " algorithm";
//     }
// }

// TEST_F(MSTTest, AnalysisFunctions)
// {
//     for (const auto &algo : algorithms)
//     {
//         auto mstAlgo = MSTFactory::createMSTAlgorithm(algo);
//         ASSERT_NE(mstAlgo, nullptr) << "Failed to create " << algo << " algorithm";

//         auto mst = mstAlgo->findMST(*g);

//         // auto totalWeightFuture = calculateTotalWeightAsync(mst);
//         // auto longestDistanceFuture = findLongestDistanceAsync(*g, mst);
//         // auto averageDistanceFuture = calculateAverageDistanceAsync(*g, mst);
//         // auto shortestMSTEdgeFuture = findShortestMSTEdgeAsync(mst);

//         // EXPECT_EQ(totalWeightFuture.get(), 16) << "Incorrect total weight for " << algo;
//         // EXPECT_GT(longestDistanceFuture.get(), 0) << "Invalid longest distance for " << algo;
//         // EXPECT_GT(averageDistanceFuture.get(), 0) << "Invalid average distance for " << algo;
//         // EXPECT_GT(shortestMSTEdgeFuture.get(), 0) << "Invalid shortest MST edge for " << algo;
//     }
// }

// int main(int argc, char **argv)
// {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }