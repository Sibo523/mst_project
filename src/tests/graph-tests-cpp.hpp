// tests/GraphTests.cpp
#include "../src/graph/Graph.hpp"
#include <cassert>
#include <stdexcept>
#include <iostream>

void testGraphConstruction() {
    Graph g(5);
    assert(g.getVertices() == 5);
    std::cout << "Graph construction test passed.\n";
}

void testAddEdge() {
    Graph g(5);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 20);
    
    const auto& neighbors = g.getNeighbors(0);
    assert(neighbors.size() == 2);
    assert(neighbors[0] == std::make_pair(1, 10));
    assert(neighbors[1] == std::make_pair(2, 20));
    std::cout << "Add edge test passed.\n";
}

void testInvalidVertex() {
    Graph g(5);
    try {
        g.addEdge(0, 5, 10);
        assert(false); // Should not reach here
    } catch (const std::out_of_range& e) {
        // Expected exception
    }
    std::cout << "Invalid vertex test passed.\n";
}

void testGetNeighbors() {
    Graph g(3);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 20);
    g.addEdge(1, 2, 30);

    const auto& neighbors0 = g.getNeighbors(0);
    assert(neighbors0.size() == 2);
    assert(neighbors0[0] == std::make_pair(1, 10));
    assert(neighbors0[1] == std::make_pair(2, 20));

    const auto& neighbors1 = g.getNeighbors(1);
    assert(neighbors1.size() == 1);
    assert(neighbors1[0] == std::make_pair(2, 30));

    const auto& neighbors2 = g.getNeighbors(2);
    assert(neighbors2.empty());

    std::cout << "Get neighbors test passed.\n";
}

// int main() {
//     testGraphConstruction();
//     testAddEdge();
//     testInvalidVertex();
//     testGetNeighbors();

//     std::cout << "All Graph tests passed!\n";
//     return 0;
// }
