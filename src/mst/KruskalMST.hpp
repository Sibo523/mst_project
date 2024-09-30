#pragma once
#include "MSTAlgorithm.hpp"
#include <vector>
#include <iostream>

class KruskalMST : public MSTAlgorithm
{
public:
    std::vector<std::pair<int, std::pair<int, int>>> findMST(const Graph &graph) override;

private:
    // Helper functions for Kruskal's algorithm
    int find(std::vector<int> &parent, int i);
    void unionSet(std::vector<int> &parent, std::vector<int> &rank, int x, int y);
};

// Helper struct to represent an edge
struct Edge
{
    int src, dest, weight;
    Edge(int s, int d, int w) : src(s), dest(d), weight(w) {}
};

// Comparison function to sort edges
struct CompareEdge
{
    bool operator()(const Edge &e1, const Edge &e2)
    {
        return e1.weight < e2.weight;
    }
};
