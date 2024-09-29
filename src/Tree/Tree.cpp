// Tree.cpp
#include "Tree.hpp"

Tree::Tree(int vertices) : V(vertices), adjList(vertices) {
    if (vertices <= 0) {
        throw std::invalid_argument("Number of vertices must be positive");
    }
}

void Tree::addEdge(int src, int dest, int weight) {
    if (src < 0 || src >= V || dest < 0 || dest >= V) {
        throw std::out_of_range("Vertex index out of bounds");
    }
    // For a tree, we need to ensure that adding this edge doesn't create a cycle
    // This is a simplified version; a real implementation would need more checks
    adjList[src].emplace_back(dest, weight);
    adjList[dest].emplace_back(src, weight);
}

const std::vector<std::pair<int, int>>& Tree::getNeighbors(int v) const {
    if (v < 0 || v >= V) {
        throw std::out_of_range("Vertex index out of bounds");
    }
    return adjList[v];
}

std::vector<std::pair<int, std::pair<int, int>>> Tree::getEdges() const {
    std::vector<std::pair<int, std::pair<int, int>>> edges;
    for (int i = 0; i < V; ++i) {
        for (const auto& neighbor : adjList[i]) {
            if (i < neighbor.first) {  // Add each edge only once
                edges.emplace_back(i, std::make_pair(neighbor.first, neighbor.second));
            }
        }
    }
    return edges;
}