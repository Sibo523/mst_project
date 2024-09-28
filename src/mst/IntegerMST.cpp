// src/mst/IntegerMST.cpp
#include "IntegerMST.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

std::vector<std::pair<int, std::pair<int, int>>> IntegerMST::findMST(const Graph& graph) {
    int V = graph.getVertices();
    std::vector<std::pair<int, std::pair<int, int>>> mst;
    std::vector<int> parent(V);
    std::vector<int> rank(V, 0);

    // Initialize disjoint set
    for (int i = 0; i < V; i++) {
        parent[i] = i;
    }

    int maxWeight = getMaxWeight(graph);
    int bucketSize = std::max(1, static_cast<int>(std::sqrt(maxWeight)));
    std::vector<std::vector<std::pair<int, std::pair<int, int>>>> buckets(bucketSize + 1);

    // Distribute edges into buckets
    for (int i = 0; i < V; i++) {
        for (const auto& [dest, weight] : graph.getNeighbors(i)) {
            if (i < dest) {  // Avoid adding edges twice
                int bucket = weight / bucketSize;
                buckets[bucket].push_back({weight, {i, dest}});
            }
        }
    }

    // Process buckets
    for (const auto& bucket : buckets) {
        for (const auto& edge : bucket) {
            int weight = edge.first;
            int u = edge.second.first;
            int v = edge.second.second;

            int set_u = find(parent, u);
            int set_v = find(parent, v);

            if (set_u != set_v) {
                mst.push_back(edge);
                unionSet(parent, rank, set_u, set_v);
            }

            if (mst.size() == V - 1) {
                return mst;
            }
        }
    }

    return mst;
}

int IntegerMST::find(std::vector<int>& parent, int i) {
    if (parent[i] != i) {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void IntegerMST::unionSet(std::vector<int>& parent, std::vector<int>& rank, int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);

    if (rank[xroot] < rank[yroot]) {
        parent[xroot] = yroot;
    } else if (rank[xroot] > rank[yroot]) {
        parent[yroot] = xroot;
    } else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

int IntegerMST::getMaxWeight(const Graph& graph) {
    int maxWeight = 0;
    for (int i = 0; i < graph.getVertices(); i++) {
        for (const auto& [dest, weight] : graph.getNeighbors(i)) {
            maxWeight = std::max(maxWeight, weight);
        }
    }
    return maxWeight;
}