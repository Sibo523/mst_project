// src/mst/MSTAlgorithm.hpp
#pragma once
#include "../graph/Graph.hpp"
#include <vector>

class MSTAlgorithm {
public:
    virtual std::vector<std::pair<int, std::pair<int, int>>> findMST(const Graph& graph) = 0;
    virtual ~MSTAlgorithm() = default;
};
