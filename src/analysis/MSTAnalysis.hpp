// src/analysis/MSTAnalysis.hpp
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

std::future<int> calculateTotalWeightAsync(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
std::future<int> findLongestDistanceAsync(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst);
std::future<double> calculateAverageDistanceAsync(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst);
std::future<int> findShortestMSTEdgeAsync(const std::vector<std::pair<int, std::pair<int, int>>> &mst);

// You might also want to keep the non-async versions if they're used elsewhere
// int calculateTotalWeight(const std::vector<std::pair<int, std::pair<int, int>>>& mst);
// int findLongestDistance(const Graph& graph, const std::vector<std::pair<int, std::pair<int, int>>>& mst);
// double calculateAverageDistance(const Graph& graph, const std::vector<std::pair<int, std::pair<int, int>>>& mst);
// int findShortestMSTEdge(const std::vector<std::pair<int, std::pair<int, int>>>& mst);