// src/mst/BoruvkaMST.cpp
#include "BoruvkaMST.hpp"
#include <algorithm>
#include <limits>

std::vector<std::pair<int, std::pair<int, int>>> BoruvkaMST::findMST(const Graph& graph) {
    int V = graph.getVertices();
    std::vector<std::pair<int, std::pair<int, int>>> mst;
    std::vector<int> parent(V);
    std::vector<int> rank(V, 0);

    // Initialize each vertex as a separate component
    for (int i = 0; i < V; i++) {
        parent[i] = i;
    }

    int numComponents = V;

    while (numComponents > 1) {
        std::vector<int> cheapest(V, -1);

        // Find the cheapest edge for each component
        for (int i = 0; i < V; i++) {
            for (const auto& [dest, weight] : graph.getNeighbors(i)) {
                int set1 = find(parent, i);
                int set2 = find(parent, dest);

                if (set1 != set2) {
                    if (cheapest[set1] == -1 || weight < graph.getNeighbors(cheapest[set1])[0].second) {
                        cheapest[set1] = i;
                    }
                    if (cheapest[set2] == -1 || weight < graph.getNeighbors(cheapest[set2])[0].second) {
                        cheapest[set2] = i;
                    }
                }
            }
        }

        // Add the cheapest edges to the MST
        for (int i = 0; i < V; i++) {
            if (cheapest[i] != -1) {
                int set1 = find(parent, i);
                int set2 = find(parent, cheapest[i]);

                if (set1 != set2) {
                    mst.push_back({graph.getNeighbors(cheapest[i])[0].second, {cheapest[i], i}});
                    unite(parent, rank, set1, set2);
                    numComponents--;
                }
            }
        }
    }

    return mst;
}

int BoruvkaMST::find(std::vector<int>& parent, int i) {
    if (parent[i] != i) {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void BoruvkaMST::unite(std::vector<int>& parent, std::vector<int>& rank, int x, int y) {
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
