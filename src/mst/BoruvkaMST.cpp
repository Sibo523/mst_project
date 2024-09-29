// src/mst/BoruvkaMST.cpp
#include "BoruvkaMST.hpp"
#include <algorithm>
#include <limits>

std::vector<std::pair<int, std::pair<int, int>>> BoruvkaMST::findMST(const Graph &graph) {
    int V = graph.getVertices();
    std::vector<std::pair<int, std::pair<int, int>>> result;
    std::vector<int> parent(V);
    std::vector<int> rank(V, 0);
    std::vector<std::pair<int, std::pair<int, int>>> cheapest(V, {std::numeric_limits<int>::max(), {-1, -1}});

    // Initialize each vertex as a separate component
    for (int i = 0; i < V; i++) {
        parent[i] = i;
    }

    int numTrees = V;
    int MSTweight = 0;

    while (numTrees > 1) {
        // Reset cheapest edges
        std::fill(cheapest.begin(), cheapest.end(), std::make_pair(std::numeric_limits<int>::max(), std::make_pair(-1, -1)));

        // Find the cheapest edge for each component
        for (int i = 0; i < V; i++) {
            for (const auto &edge : graph.getNeighbors(i)) {
                int dest = edge.first;
                int weight = edge.second;

                int set1 = find(parent, i);
                int set2 = find(parent, dest);

                if (set1 != set2) {
                    if (weight < cheapest[set1].first) {
                        cheapest[set1] = {weight, {i, dest}};
                    }
                    if (weight < cheapest[set2].first) {
                        cheapest[set2] = {weight, {i, dest}};
                    }
                }
            }
        }

        // Add the cheapest edges to the MST
        for (int i = 0; i < V; i++) {
            if (cheapest[i].second.first != -1) {
                int set1 = find(parent, cheapest[i].second.first);
                int set2 = find(parent, cheapest[i].second.second);

                if (set1 != set2) {
                    MSTweight += cheapest[i].first;
                    unite(parent, rank, set1, set2);
                    result.push_back({cheapest[i].first, {cheapest[i].second.first, cheapest[i].second.second}});
                    numTrees--;
                }
            }
        }
    }

    return result;
}

int BoruvkaMST::find(std::vector<int> &parent, int i) {
    if (parent[i] != i) {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void BoruvkaMST::unite(std::vector<int> &parent, std::vector<int> &rank, int x, int y) {
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