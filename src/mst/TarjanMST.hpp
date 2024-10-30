#pragma once
#include "MSTAlgorithm.hpp"
#include <vector>
#include <random>
#include <iostream>

class TarjanMST : public MSTAlgorithm
{
public:
    std::vector<std::pair<int, std::pair<int, int>>> findMST(const Graph &graph) override;

private:
    struct Edge
    {
        int src, dest, weight;
        Edge(int s, int d, int w) : src(s), dest(d), weight(w) {}
        Edge() : src(0), dest(0), weight(0) {} // Default constructor for std::vector
    };
    // Helper functions for Tarjan's algorithm
    void iterativeMST(const Graph &graph, std::vector<std::pair<int, std::pair<int, int>>> &mst,
                      std::vector<int> &parent, std::vector<int> &rank,
                      std::vector<Edge> &edges);
    int find(std::vector<int> &parent, int i);
    void unionSet(std::vector<int> &parent, std::vector<int> &rank, int x, int y);
    std::vector<Edge> sampleEdges(const std::vector<Edge> &edges, int k);
};
