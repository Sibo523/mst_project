#pragma once

#include "../graph/Graph.hpp"
#include <vector>
#include <utility>
#include <future>

struct MSTAnalysis
{

    std::vector<std::pair<int, std::pair<int, int>>> mstEdges;
    static std::pair<int, double> calculateTotalWeight(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
    static std::pair<int, double> findLongestDistance(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
    static std::pair<int, double> calculateAverageDistance(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
    static std::pair<int, double> findShortestMSTEdge(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
    static std::vector<std::function<void(void *)>> getPipelineFunctions();
};
