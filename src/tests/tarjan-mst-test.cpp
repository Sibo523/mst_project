#include <gtest/gtest.h>
#include "../mst/TarjanMST.hpp"

class TarjanMSTTest : public ::testing::Test {
protected:
    void SetUp() override {
        tarjan = std::make_unique<TarjanMST>();
    }

    void TearDown() override {
        tarjan.reset();
    }

    std::unique_ptr<TarjanMST> tarjan;
};

TEST_F(TarjanMSTTest, EmptyGraph) {
    Graph g(0);
    auto result = tarjan->findMST(g);
    ASSERT_TRUE(result.empty());
}

TEST_F(TarjanMSTTest, SingleVertex) {
    Graph g(1);
    auto result = tarjan->findMST(g);
    ASSERT_TRUE(result.empty());
}

TEST_F(TarjanMSTTest, SimpleGraph) {
    Graph g(3);
    g.addEdge(0, 1, 2);
    g.addEdge(1, 2, 3);
    g.addEdge(0, 2, 4);

    auto result = tarjan->findMST(g);
    ASSERT_EQ(result.size(), 2);
    
    int totalWeight = 0;
    for (const auto& edge : result) {
        totalWeight += edge.first;
    }
    ASSERT_EQ(totalWeight, 5);
}

TEST_F(TarjanMSTTest, LargerGraph) {
    Graph g(5);
    // Create a more complex graph to test edge sampling
    g.addEdge(0, 1, 2);
    g.addEdge(1, 2, 3);
    g.addEdge(2, 3, 4);
    g.addEdge(3, 4, 5);
    g.addEdge(0, 4, 6);
    g.addEdge(0, 2, 7);
    g.addEdge(1, 3, 8);

    auto result = tarjan->findMST(g);
    ASSERT_EQ(result.size(), 4);  // Should have V-1 edges
}

TEST_F(TarjanMSTTest, DisconnectedGraph) {
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(2, 3, 2);

    auto result = tarjan->findMST(g);
    ASSERT_EQ(result.size(), 2);
}
