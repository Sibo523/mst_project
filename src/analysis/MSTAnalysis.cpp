#include "MSTAnalysis.hpp"
#include <algorithm>
#include <limits>
#include <queue>
#include <vector>
#include <iostream>
#include <future>

std::future<int> calculateTotalWeightAsync(const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    return std::async(std::launch::async, [&mst]()
                      {
        if (mst.empty())
        {
            std::cerr << "Warning: Empty MST passed to calculateTotalWeight" << std::endl;
            return 0;
        }

        int totalWeight = 0;
        for (const auto& edge : mst)
        {
            if (edge.first < 0)
            {
                std::cerr << "Warning: Negative weight encountered in MST" << std::endl;
                continue;
            }
            totalWeight += edge.first;
        }
        return totalWeight; });
}
// folyd warshall algorithm but now we do the opposite, we find the longest distance
std::future<int> findLongestDistanceAsync(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    return std::async(std::launch::async, [&graph, &mst]()
                      {
        int V = graph.getVertices();
        std::vector<std::vector<int>> dist(V, std::vector<int>(V, std::numeric_limits<int>::max()));

        // Initialize distances with MST edges
        for (const auto& edge : mst)
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
        //for debug
        // std::cout << "Longest distance: " << maxDist << std::endl;
        return maxDist; });
}
// also floyd warsahll
std::future<double> calculateAverageDistanceAsync(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    return std::async(std::launch::async, [&graph, &mst]()
                      {
        int V = graph.getVertices();
        std::vector<std::vector<int>> dist(V, std::vector<int>(V, std::numeric_limits<int>::max()));

        // Initialize distances with MST edges
        for (const auto& edge : mst)
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
        //for debug
        // std::cout << "Average distance: " << (count > 0 ? static_cast<double>(sum) / count : 0.0) << std::endl;
        return count > 0 ? static_cast<double>(sum) / count : 0.0; });
}

std::future<int> findShortestMSTEdgeAsync(const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    return std::async(std::launch::async, [&mst]()
                      {
        if (mst.empty())
            return 0;

        int shortestEdge = std::numeric_limits<int>::max();
        for (const auto& edge : mst)
        {
            shortestEdge = std::min(shortestEdge, edge.first);
        }
        // std::cout << "Shortest MST edge: " << shortestEdge << std::endl;
        return shortestEdge; });
}

std::future<MSTAnalysis> analyzeMSTAsync(const Graph &graph, const std::vector<std::pair<int, std::pair<int, int>>> &mst)
{
    // this is the functino that is being called from outside (server in this instance)
    return std::async(std::launch::async, [&graph, &mst]()
                      {
        MSTAnalysis analysis;

        auto totalWeightFuture = calculateTotalWeightAsync(mst);
        auto longestDistanceFuture = findLongestDistanceAsync(graph, mst);
        auto averageDistanceFuture = calculateAverageDistanceAsync(graph, mst);
        auto shortestMSTEdgeFuture = findShortestMSTEdgeAsync(mst);

        analysis.totalWeight = totalWeightFuture.get();
        analysis.longestDistance = longestDistanceFuture.get();
        analysis.averageDistance = averageDistanceFuture.get();
        analysis.shortestMSTEdge = shortestMSTEdgeFuture.get();
        analysis.mstEdges = mst;

        return analysis; });
}