// src/mst/KruskalMST.cpp
#include "KruskalMST.hpp"
#include <algorithm>

std::vector<std::pair<int, std::pair<int, int>>> KruskalMST::findMST(const Graph &graph)
{
    int V = graph.getVertices();
    if (V == 0)
    {
        std::cout << "Warning: Empty graph passed to KruskalMST::findMST" << std::endl;
        return {}; // Return empty MST for empty graph
    }
    std::vector<Edge> edges;
    std::vector<std::pair<int, std::pair<int, int>>> mst;

    // Collect all edges from the graph
    for (int i = 0; i < V; i++)
    {
        for (const auto &[dest, weight] : graph.getNeighbors(i))
        {
            edges.emplace_back(i, dest, weight);
        }
    }

    // Sort edges in non-decreasing order of their weight
    std::sort(edges.begin(), edges.end(), CompareEdge());

    // Create disjoint sets
    std::vector<int> parent(V);
    std::vector<int> rank(V, 0);
    for (int i = 0; i < V; i++)
    {
        parent[i] = i;
    }

    // Process edges in sorted order
    for (const auto &edge : edges)
    {
        int x = find(parent, edge.src);
        int y = find(parent, edge.dest);

        // If including this edge doesn't create a cycle, include it in MST
        if (x != y)
        {
            mst.push_back({edge.weight, {edge.src, edge.dest}});
            unionSet(parent, rank, x, y);
        }
    }

    return mst;
}

int KruskalMST::find(std::vector<int> &parent, int i)
{
    if (parent[i] != i)
    {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void KruskalMST::unionSet(std::vector<int> &parent, std::vector<int> &rank, int x, int y)
{
    int xroot = find(parent, x);
    int yroot = find(parent, y);

    // Attach smaller rank tree under root of high rank tree
    if (rank[xroot] < rank[yroot])
    {
        parent[xroot] = yroot;
    }
    else if (rank[xroot] > rank[yroot])
    {
        parent[yroot] = xroot;
    }
    // If ranks are same, then make one as root and increment its rank
    else
    {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}
