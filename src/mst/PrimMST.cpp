// src/mst/PrimMST.cpp
#include "PrimMST.hpp"
#include <limits>

std::vector<std::pair<int, std::pair<int, int>>> PrimMST::findMST(const Graph& graph) {
    int V = graph.getVertices();
    std::vector<std::pair<int, std::pair<int, int>>> mst;
    std::vector<int> parent(V, -1);
    std::vector<int> key(V, std::numeric_limits<int>::max());
    std::vector<bool> mstSet(V, false);

    // Start with the first vertex
    key[0] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = getMinKeyVertex(key, mstSet, V);
        mstSet[u] = true;

        for (const auto& [v, weight] : graph.getNeighbors(u)) {
            if (!mstSet[v] && weight < key[v]) {
                parent[v] = u;
                key[v] = weight;
            }
        }
    }

    // Construct MST
    for (int i = 1; i < V; i++) {
        mst.push_back({key[i], {parent[i], i}});
    }

    return mst;
}

int PrimMST::getMinKeyVertex(const std::vector<int>& key, const std::vector<bool>& mstSet, int V) {
    int min = std::numeric_limits<int>::max();
    int min_index = -1;

    for (int v = 0; v < V; v++) {
        if (!mstSet[v] && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }

    return min_index;
}
