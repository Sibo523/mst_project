// src/analysis/MSTAnalysis.hpp
#pragma once
#include "../graph/Graph.hpp"
#include <vector>

struct MSTAnalysis
{
    int totalWeight;
    int longestDistance;
    double averageDistance;
    int shortestMSTEdge;
    explicit MSTAnalysis(const Graph &graph);
    // defult constructor
    MSTAnalysis() = default;
};

MSTAnalysis analyzeMST(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst);

// Helper function declarations
int calculateTotalWeight(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
int findLongestDistance(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst);
double calculateAverageDistance(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst);
int findShortestMSTEdge(const std::vector<std::pair<int, std::pair<int, int>>> &mst);
