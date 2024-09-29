// src/graph/Graph.cpp
#include "Graph.hpp"

Graph::Graph(int vertices) : V(vertices), adjList(vertices) {}

void Graph::addEdge(int src, int dest, int weight)
{
    if (src < 0 || src >= V || dest < 0 || dest >= V)
    {
        throw std::out_of_range("Vertex index out of range");
    }
    adjList[src].emplace_back(dest, weight);
}
std::vector<std::pair<int, std::pair<int, int>>> Graph::getEdges() const
{
    std::vector<std::pair<int, std::pair<int, int>>> edges;
    for (int i = 0; i < V; i++)
    {
        for (const auto &neighbor : adjList[i])
        {
            edges.emplace_back(i, std::make_pair(neighbor.first, neighbor.second));
        }
    }
    return edges;
}
const std::vector<std::pair<int, int>> &Graph::getNeighbors(int v) const
{
    if (v < 0 || v >= V)
    {
        throw std::out_of_range("Vertex index out of range");
    }
    return adjList[v];
}
