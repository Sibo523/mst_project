// src/analysis/MSTAnalysis.cpp
#include "MSTAnalysis.hpp"
#include <algorithm>
#include <limits>
#include <queue>
#include <vector>
MSTAnalysis::MSTAnalysis(const Graph &graph)
{
    // Initialize the analysis based on the graph
    // This is just an example, adjust according to your needs
    totalWeight = 0;
    longestDistance = 0;
    averageDistance = 0.0;
    shortestMSTEdge = std::numeric_limits<int>::max();
    // You might want to perform some initial analysis here
}
MSTAnalysis analyzeMST(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    MSTAnalysis analysis;
    analysis.totalWeight = calculateTotalWeight(mst);
    analysis.longestDistance = findLongestDistance(graph, mst);
    analysis.averageDistance = calculateAverageDistance(graph, mst);
    analysis.shortestMSTEdge = findShortestMSTEdge(mst);
    return analysis;
}

int calculateTotalWeight(const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    int totalWeight = 0;
    for (const auto &edge : mst)
    {
        totalWeight += edge.first;
    }
    return totalWeight;
}

int findLongestDistance(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    int V = graph.getVertices();
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

    return maxDist;
}

double calculateAverageDistance(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    int V = graph.getVertices();
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

    return count > 0 ? static_cast<double>(sum) / count : 0.0;
}

int findShortestMSTEdge(const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    if (mst.empty())
        return 0;

    int shortestEdge = std::numeric_limits<int>::max();
    for (const auto &edge : mst)
    {
        shortestEdge = std::min(shortestEdge, edge.first);
    }
    return shortestEdge;
}
