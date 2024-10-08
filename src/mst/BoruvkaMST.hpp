#pragma once
#include "MSTAlgorithm.hpp"
#include <vector>
#include <iostream>

class BoruvkaMST : public MSTAlgorithm
{
public:
    std::vector<std::pair<int, std::pair<int, int>>> findMST(const Graph &graph) override;

private:
    // Helper functions for Borůvka's algorithm
    int find(std::vector<int> &parent, int i);
    void unite(std::vector<int> &parent, std::vector<int> &rank, int x, int y);
};
