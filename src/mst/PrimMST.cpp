// src/mst/PrimMST.cpp
#include "PrimMST.hpp"
#include <limits>
#include <ostream>
#include <queue>
#include <iostream>
std::vector<std::pair<int, std::pair<int, int>>> PrimMST::findMST(const Graph &graph)
{
    std::vector<std::pair<int, std::pair<int, int>>> mst;
    int V = graph.getVertices();

    if (V == 0)
    {
        std::cout << "Warning: Empty graph passed to PrimMST::findMST" << std::endl;
        return mst; // Return empty MST for empty graph
    }
    std::vector<bool> inMST(V, false);
    std::vector<int> key(V, std::numeric_limits<int>::max());
    std::vector<int> parent(V, -1);

    // Use a min-heap priority queue
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

    // Start with the first vertex
    pq.push({0, 0});
    key[0] = 0;

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        if (inMST[u])
            continue;

        inMST[u] = true;

        // Add edge to MST
        if (parent[u] != -1)
        {
            mst.push_back({key[u], {parent[u], u}});
        }

        // Update key values of adjacent vertices
        for (const auto &[v, weight] : graph.getNeighbors(u))
        {
            if (!inMST[v] && weight < key[v])
            {
                parent[v] = u;
                key[v] = weight;
                pq.push({key[v], v});
            }
        }
    }

    return mst;
}
int PrimMST::getMinKeyVertex(const std::vector<int> &key, const std::vector<bool> &mstSet, int V)
{
    int min = std::numeric_limits<int>::max();
    int min_index = -1;

    for (int v = 0; v < V; v++)
    {
        if (!mstSet[v] && key[v] < min)
        {
            min = key[v];
            min_index = v;
        }
    }

    return min_index;
}
