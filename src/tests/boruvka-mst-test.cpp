#include <gtest/gtest.h>
#include "../mst/BoruvkaMST.hpp"

class BoruvkaMSTTest : public ::testing::Test {
protected:
    void SetUp() override {
        boruvka = std::make_unique<BoruvkaMST>();
    }

    void TearDown() override {
        boruvka.reset();
    }

    std::unique_ptr<BoruvkaMST> boruvka;
};

TEST_F(BoruvkaMSTTest, EmptyGraph) {
    Graph g(0);
    auto result = boruvka->findMST(g);
    ASSERT_TRUE(result.empty());
}

TEST_F(BoruvkaMSTTest, SingleVertex) {
    Graph g(1);
    auto result = boruvka->findMST(g);
    ASSERT_TRUE(result.empty());
}

TEST_F(BoruvkaMSTTest, SimpleConnectedGraph) {
    Graph g(3);
    g.addEdge(0, 1, 2);
    g.addEdge(1, 2, 3);
    g.addEdge(0, 2, 4);

    auto result = boruvka->findMST(g);
    ASSERT_EQ(result.size(), 2);

    int totalWeight = 0;
    for (const auto& edge : result) {
        totalWeight += edge.first;
    }
    ASSERT_EQ(totalWeight, 5);
}

// TEST_F(BoruvkaMSTTest, MultipleComponents) {
//     Graph g(6);
//     // Create three components
//     g.addEdge(0, 1, 1);
//     g.addEdge(2, 3, 2);
//     g.addEdge(4, 5, 3);

//     auto result = boruvka->findMST(g);
//     ASSERT_EQ(result.size(), 3);
// }

// TEST_F(BoruvkaMSTTest, CyclicGraph) {
//     Graph g(4);
//     g.addEdge(0, 1, 1);
//     g.addEdge(1, 2, 2);
//     g.addEdge(2, 3, 3);
//     g.addEdge(3, 0, 4);

//     auto result = boruvka->findMST(g);
//     ASSERT_EQ(result.size(), 3);
    
//     // Verify the total weight
//     int totalWeight = 0;
//     for (const auto& edge : result) {
//         totalWeight += edge.first;
//     }
//     ASSERT_EQ(totalWeight, 6);  // 1 + 2 + 3
// }
