#include "MSTFactory.hpp"
#include "../mst/BoruvkaMST.hpp"
#include "../mst/PrimMST.hpp"
#include "../mst/KruskalMST.hpp"
#include "../mst/TarjanMST.hpp"
#include <stdexcept>

// simple factory pattern
std::unique_ptr<MSTAlgorithm> MSTFactory::createMSTAlgorithm(const std::string &algorithmName)
{
    if (algorithmName == "Boruvka")
    {
        return std::make_unique<BoruvkaMST>();
    }
    else if (algorithmName == "Prim")
    {
        return std::make_unique<PrimMST>();
    }
    else if (algorithmName == "Kruskal")
    {
        return std::make_unique<KruskalMST>();
    }
    else if (algorithmName == "Tarjan")
    {
        return std::make_unique<TarjanMST>();
    }
    else
    {
        throw std::invalid_argument("Unknown MST algorithm: " + algorithmName);
    }
}