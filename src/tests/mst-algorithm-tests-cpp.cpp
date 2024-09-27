// tests/MSTAlgorithmTests.cpp
#include "../src/mst/BoruvkaMST.hpp"
#include "../src/mst/PrimMST.hpp"
#include "../src/mst/KruskalMST.hpp"
#include "../src/mst/TarjanMST.hpp"
#include "../src/factory/MSTFactory.hpp"
#include <cassert>
#include <iostream>
#include <set>

// Helper function to create a test graph
Graph createTestGraph() {
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 6);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 4);
    return g;
}

// Helper function to verify MST
void verifyMST(const std::vector<std::pair<int, std::pair<int, int>>>& mst) {
    assert(mst.size() == 3);
    int totalWeight = 0;
    std::set<std::pair<int, int>> edges;
    for (const auto& edge : mst) {
        totalWeight += edge.first;
        edges.insert(edge.second);
    }
    assert(totalWeight == 15);
    assert(edges.count({0, 3}) == 1);
    assert(edges.count({2, 3}) == 1);
    assert(edges.count({0, 1}) == 1 || edges.count({0, 2}) == 1);
}

void testBoruvkaMST() {
    Graph g = createTestGraph();
    BoruvkaMST boruvka;
    auto mst = boruvka.findMST(g);
    verifyMST(mst);
    std::cout << "Boruvka's MST test passed.\n";
}

void testPrimMST() {
    Graph g = createTestGraph();
    PrimMST prim;
    auto mst = prim.findMST(g);
    verifyMST(mst);
    std::cout << "Prim's MST test passed.\n";
}

void testKruskalMST() {
    Graph g = createTestGraph();
    KruskalMST kruskal;
    auto mst = kruskal.findMST(g);
    verifyMST(mst);
    std::cout << "Kruskal's MST test passed.\n";
}

void testTarjanMST() {
    Graph g = createTestGraph();
    TarjanMST tarjan;
    auto mst = tarjan.findMST(g);
    verifyMST(mst);
    std::cout << "Tarjan's MST test passed.\n";
}

void testMSTFactory() {
    std::vector<std::string> algorithms = {"Boruvka", "Prim", "Kruskal", "Tarjan"};
    Graph g = createTestGraph();
    
    for (const auto& algo : algorithms) {
        auto mstAlgo = MSTFactory::createMSTAlgorithm(algo);
        auto mst = mstAlgo->findMST(g);
        verifyMST(mst);
    }
    
    std::cout << "MST Factory test passed.\n";
}

// int main() {
//     testBoruvkaMST();
//     testPrimMST();
//     testKruskalMST();
//     testTarjanMST();
//     testMSTFactory();

//     std::cout << "All MST Algorithm tests passed!\n";
//     return 0;
// }
