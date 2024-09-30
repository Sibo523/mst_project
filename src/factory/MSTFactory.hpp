#pragma once
#include "../mst/MSTAlgorithm.hpp"
#include <memory>
#include <string>

class MSTFactory {
public:
    static std::unique_ptr<MSTAlgorithm> createMSTAlgorithm(const std::string& algorithmName);
};