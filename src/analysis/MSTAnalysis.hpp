#pragma once

#include "../graph/Graph.hpp"
#include <vector>
#include <utility>
#include <future>

struct MSTAnalysis
{
    static std::vector<std::function<std::pair<int, double>(const std::vector<std::pair<int, std::pair<int, int>>> &)>> getPipelineFunctions()
    {
        return {
            calculateTotalWeight,
            findLongestDistance,
            calculateAverageDistance,
            findShortestMSTEdge};
    }

    static std::pair<int, double> calculateTotalWeight(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
    static std::pair<int, double> findLongestDistance(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
    static std::pair<int, double> calculateAverageDistance(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
    static std::pair<int, double> findShortestMSTEdge(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
    static std::string getAllResults(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
};