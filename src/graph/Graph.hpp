#pragma once
#include <vector>
#include <stdexcept>

class Graph
{
public:
    // nothing special just a graph class
    Graph(int vertices);
    void addEdge(int src, int dest, int weight);
    int getVertices() const { return V; }
    const std::vector<std::pair<int, int>> &getNeighbors(int v) const;
    std::vector<std::pair<int, std::pair<int, int>>> getEdges() const;

private:
    int V;                                                 // Number of vertices
    std::vector<std::vector<std::pair<int, int>>> adjList; // Adjacency list
};