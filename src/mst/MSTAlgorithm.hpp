#pragma once
#include "../graph/Graph.hpp"
#include <vector>

class MSTAlgorithm
{ // abstract class akk the mst/ files inherit from this
    // I am simply building the structure of the classes here
    // all the algorithms do the same thing but in different ways
public:
    virtual std::vector<std::pair<int, std::pair<int, int>>> findMST(const Graph &graph) = 0;
    virtual ~MSTAlgorithm() = default;
};
