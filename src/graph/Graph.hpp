// src/graph/Graph.hpp
#pragma once
#include <vector>
#include <stdexcept>

class Graph {
public:
    Graph(int vertices);
    void addEdge(int src, int dest, int weight);
    int getVertices() const { return V; }
    const std::vector<std::pair<int, int>>& getNeighbors(int v) const;

private:
    int V;  // Number of vertices
    std::vector<std::vector<std::pair<int, int>>> adjList;  // Adjacency list
};
