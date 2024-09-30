#pragma once
#include "MSTAlgorithm.hpp"
#include <queue>

class PrimMST : public MSTAlgorithm {
public:
    std::vector<std::pair<int, std::pair<int, int>>> findMST(const Graph& graph) override;

private:
    // Helper function to get the minimum key vertex
    int getMinKeyVertex(const std::vector<int>& key, const std::vector<bool>& mstSet, int V);
};
