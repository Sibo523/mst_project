// src/mst/IntegerMST.hpp
#pragma once
#include "MSTAlgorithm.hpp"
#include <vector>

class IntegerMST : public MSTAlgorithm {
public:
    std::vector<std::pair<int, std::pair<int, int>>> findMST(const Graph& graph) override;

private:
    // Helper functions for Integer MST algorithm
    int find(std::vector<int>& parent, int i);
    void unionSet(std::vector<int>& parent, std::vector<int>& rank, int x, int y);
    int getMaxWeight(const Graph& graph);
};