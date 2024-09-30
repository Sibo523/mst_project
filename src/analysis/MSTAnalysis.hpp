#pragma once

#include "../graph/Graph.hpp"
#include <vector>
#include <utility>
#include <future>

struct MSTAnalysis
{
    int totalWeight;
    int longestDistance;
    double averageDistance;
    int shortestMSTEdge;
    std::vector<std::pair<int, std::pair<int, int>>> mstEdges;
};
// doing all this async like we have been asked
std::future<int> calculateTotalWeightAsync(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
std::future<int> findLongestDistanceAsync(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst);
std::future<double> calculateAverageDistanceAsync(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst);
std::future<int> findShortestMSTEdgeAsync(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
