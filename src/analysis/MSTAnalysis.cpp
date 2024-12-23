#include "MSTAnalysis.hpp"
#include <algorithm>
#include <limits>
#include <queue>
#include <vector>
#include <iostream>

std::pair<int, double> MSTAnalysis::calculateTotalWeight(const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    if (mst.empty())
    {
        std::cerr << "Warning: Empty MST passed to calculateTotalWeight" << std::endl;
        return std::make_pair(0, 0);
    }

    int totalWeight = 0;
    for (const auto &edge : mst)
    {
        if (edge.first < 0)
        {
            std::cerr << "Warning: Negative weight encountered in MST" << std::endl;
            continue;
        }
        totalWeight += edge.first;
    }
    return std::make_pair(0, totalWeight);
}

std::pair<int, double> MSTAnalysis::findLongestDistance(const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    int V = mst.size() + 1;
    std::vector<std::vector<int>> dist(V, std::vector<int>(V, std::numeric_limits<int>::max()));

    // Initialize distances with MST edges
    for (const auto &edge : mst)
    {
        int u = edge.second.first, v = edge.second.second, w = edge.first;
        dist[u][v] = dist[v][u] = w;
    }

    // Floyd-Warshall algorithm to find all-pairs shortest paths
    for (int k = 0; k < V; k++)
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (dist[i][k] != std::numeric_limits<int>::max() &&
                    dist[k][j] != std::numeric_limits<int>::max() &&
                    dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Find the maximum distance
    int maxDist = 0;
    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            if (dist[i][j] != std::numeric_limits<int>::max())
            {
                maxDist = std::max(maxDist, dist[i][j]);
            }
        }
    }
    return std::make_pair(1, maxDist);
}

std::pair<int, double> MSTAnalysis::calculateAverageDistance(const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    int V = mst.size() + 1;
    std::vector<std::vector<int>> dist(V, std::vector<int>(V, std::numeric_limits<int>::max()));

    // Initialize distances with MST edges
    for (const auto &edge : mst)
    {
        int u = edge.second.first, v = edge.second.second, w = edge.first;
        dist[u][v] = dist[v][u] = w;
    }

    // Floyd-Warshall algorithm to find all-pairs shortest paths
    for (int k = 0; k < V; k++)
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (dist[i][k] != std::numeric_limits<int>::max() &&
                    dist[k][j] != std::numeric_limits<int>::max() &&
                    dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Calculate average distance
    long long sum = 0;
    int count = 0;
    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            if (dist[i][j] != std::numeric_limits<int>::max())
            {
                sum += dist[i][j];
                count++;
            }
        }
    }
    return std::make_pair(2, count > 0 ? static_cast<double>(sum) / count : 0.0);
}

std::pair<int, double> MSTAnalysis::findShortestMSTEdge(const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    if (mst.empty())
    {
        return std::make_pair(3, 0);
    }

    int shortestEdge = std::numeric_limits<int>::max();
    for (const auto &edge : mst)
    {
        shortestEdge = std::min(shortestEdge, edge.first);
    }
    return std::make_pair(3, shortestEdge);
}
std::string MSTAnalysis::getAllResults(const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    std::string results;
    for (const auto &func : getPipelineFunctions())
    {
        auto result = func(mst);
        results += "Analysis " + std::to_string(result.first) + ": " + std::to_string(result.second) + "\n";
    }
    return results;
}
