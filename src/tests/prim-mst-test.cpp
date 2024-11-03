#include <gtest/gtest.h>
#include "../mst/PrimMST.hpp"

class PrimMSTTest : public ::testing::Test {
protected:
    void SetUp() override {
        prim = std::make_unique<PrimMST>();
    }

    void TearDown() override {
        prim.reset();
    }

    std::unique_ptr<PrimMST> prim;
};

TEST_F(PrimMSTTest, EmptyGraph) {
    Graph g(0);
    auto result = prim->findMST(g);
    ASSERT_TRUE(result.empty());
}

TEST_F(PrimMSTTest, SingleVertexGraph) {
    Graph g(1);
    auto result = prim->findMST(g);
    ASSERT_TRUE(result.empty());
}

TEST_F(PrimMSTTest, SimpleConnectedGraph) {
    Graph g(3);
    g.addEdge(0, 1, 2);
    g.addEdge(1, 2, 3);
    g.addEdge(0, 2, 4);

    auto result = prim->findMST(g);
    
    ASSERT_EQ(result.size(), 2);  // Should have V-1 edges
    
    int totalWeight = 0;
    for (const auto& edge : result) {
        totalWeight += edge.first;
    }
    ASSERT_EQ(totalWeight, 5);  // Minimum spanning tree weight should be 5 (2 + 3)
}

TEST_F(PrimMSTTest, CompleteGraph) {
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(0, 2, 2);
    g.addEdge(0, 3, 3);
    g.addEdge(1, 2, 4);
    g.addEdge(1, 3, 5);
    g.addEdge(2, 3, 6);

    auto result = prim->findMST(g);
    
    ASSERT_EQ(result.size(), 3);  // Should have V-1 edges
    
    int totalWeight = 0;
    for (const auto& edge : result) {
        totalWeight += edge.first;
    }
    ASSERT_EQ(totalWeight, 6);  // Minimum weight should be 6 (1 + 2 + 3)
}
