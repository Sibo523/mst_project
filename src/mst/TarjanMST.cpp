// src/mst/TarjanMST.cpp
#include "TarjanMST.hpp"
#include <algorithm>
#include <cmath>

std::vector<std::pair<int, std::pair<int, int>>> TarjanMST::findMST(const Graph &graph)
{
    int V = graph.getVertices();
    std::vector<Edge> edges;
    std::vector<std::pair<int, std::pair<int, int>>> mst;
    std::vector<int> parent(V), rank(V, 0);

    // Collect all edges from the graph
    for (int i = 0; i < V; i++)
    {
        for (const auto &[dest, weight] : graph.getNeighbors(i))
        {
            edges.emplace_back(i, dest, weight);
        }
    }

    // Initialize disjoint set
    for (int i = 0; i < V; i++)
    {
        parent[i] = i;
    }

    int m = edges.size();
    recursiveMST(graph, mst, parent, rank, edges, m);

    return mst;
}

void TarjanMST::recursiveMST(const Graph &graph, std::vector<std::pair<int, std::pair<int, int>>> &mst,
                             std::vector<int> &parent, std::vector<int> &rank,
                             std::vector<Edge> &edges, int &m)
{
    if (m <= 1)
    {
        if (m == 1)
        {
            mst.push_back({edges[0].weight, {edges[0].src, edges[0].dest}});
        }
        return;
    }

    int k = std::ceil(std::pow(m, 0.75));
    std::vector<Edge> F = sampleEdges(edges, k);

    // Recursive call on sampled edges
    recursiveMST(graph, mst, parent, rank, F, k);

    // Process remaining edges
    std::vector<Edge> remaining;
    for (const Edge &e : edges)
    {
        int x = find(parent, e.src);
        int y = find(parent, e.dest);
        if (x != y)
        {
            remaining.push_back(e);
        }
    }

    // Final processing
    for (const Edge &e : remaining)
    {
        int x = find(parent, e.src);
        int y = find(parent, e.dest);
        if (x != y)
        {
            mst.push_back({e.weight, {e.src, e.dest}});
            unionSet(parent, rank, x, y);
        }
    }

    m = remaining.size();
    edges = std::move(remaining);
}

int TarjanMST::find(std::vector<int> &parent, int i)
{
    if (parent[i] != i)
    {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void TarjanMST::unionSet(std::vector<int> &parent, std::vector<int> &rank, int x, int y)
{
    int xroot = find(parent, x);
    int yroot = find(parent, y);

    if (rank[xroot] < rank[yroot])
    {
        parent[xroot] = yroot;
    }
    else if (rank[xroot] > rank[yroot])
    {
        parent[yroot] = xroot;
    }
    else
    {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

std::vector<TarjanMST::Edge> TarjanMST::sampleEdges(const std::vector<Edge> &edges, int k)
{
    std::vector<Edge> sampled = edges;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(sampled.begin(), sampled.end(), g);
    sampled.resize(std::min(k, static_cast<int>(sampled.size())));
    return sampled;
}
