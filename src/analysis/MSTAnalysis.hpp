#pragma once

#include "../graph/Graph.hpp"
#include <vector>
#include <utility>
#include <future>

struct MSTAnalysis
{
  
    std::vector<std::pair<int, std::pair<int, int>>> mstEdges;
    static int calculateTotalWeight(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
    static int findLongestDistance(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst);
    static double calculateAverageDistance(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst);
    static std::pair<int, int> findShortestMSTEdge(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
    static std::vector<std::function<void(void *)>> getPipelineFunctions();
};
