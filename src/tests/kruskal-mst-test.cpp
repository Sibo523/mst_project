#include <gtest/gtest.h>
#include "../mst/KruskalMST.hpp"

class KruskalMSTTest : public ::testing::Test {
protected:
    void SetUp() override {
        kruskal = std::make_unique<KruskalMST>();
    }

    void TearDown() override {
        kruskal.reset();
    }

    std::unique_ptr<KruskalMST> kruskal;
};

TEST_F(KruskalMSTTest, EmptyGraph) {
    Graph g(0);
    auto result = kruskal->findMST(g);
    ASSERT_TRUE(result.empty());
}

TEST_F(KruskalMSTTest, SingleVertex) {
    Graph g(1);
    auto result = kruskal->findMST(g);
    ASSERT_TRUE(result.empty());
}

TEST_F(KruskalMSTTest, SimpleGraph) {
    Graph g(3);
    g.addEdge(0, 1, 2);
    g.addEdge(1, 2, 3);
    g.addEdge(0, 2, 4);

    auto result = kruskal->findMST(g);
    
    ASSERT_EQ(result.size(), 2);
    
    int totalWeight = 0;
    for (const auto& edge : result) {
        totalWeight += edge.first;
    }
    ASSERT_EQ(totalWeight, 5);
}

TEST_F(KruskalMSTTest, CyclicGraph) {
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(2, 3, 3);
    g.addEdge(3, 0, 4);

    auto result = kruskal->findMST(g);
    
    ASSERT_EQ(result.size(), 3);
    
    // Should not include the heaviest edge (weight 4)
    for (const auto& edge : result) {
        ASSERT_LT(edge.first, 4);
    }
}

TEST_F(KruskalMSTTest, DisconnectedComponents) {
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(2, 3, 2);

    auto result = kruskal->findMST(g);
    ASSERT_EQ(result.size(), 2);
}
