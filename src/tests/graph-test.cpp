#include <gtest/gtest.h>
#include "../graph/Graph.hpp"
#include <stdexcept>

class GraphTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup can be added here
    }
};

TEST_F(GraphTest, ConstructorWithValidVertices) {
    ASSERT_NO_THROW(Graph g(5));
}

TEST_F(GraphTest, ConstructorWithZeroVertices) {
    ASSERT_NO_THROW(Graph g(0));
}

TEST_F(GraphTest, AddValidEdge) {
    Graph g(5);
    ASSERT_NO_THROW(g.addEdge(0, 1, 10));
    
    auto neighbors = g.getNeighbors(0);
    ASSERT_EQ(neighbors.size(), 1);
    ASSERT_EQ(neighbors[0].first, 1);
    ASSERT_EQ(neighbors[0].second, 10);
}

TEST_F(GraphTest, AddEdgeOutOfRange) {
    Graph g(5);
    ASSERT_THROW(g.addEdge(5, 1, 10), std::out_of_range);
    ASSERT_THROW(g.addEdge(0, 5, 10), std::out_of_range);
    ASSERT_THROW(g.addEdge(-1, 1, 10), std::out_of_range);
}

TEST_F(GraphTest, GetNeighborsInvalidVertex) {
    Graph g(5);
    ASSERT_THROW(g.getNeighbors(-1), std::out_of_range);
    ASSERT_THROW(g.getNeighbors(5), std::out_of_range);
}

TEST_F(GraphTest, GetNeighborsEmptyVertex) {
    Graph g(5);
    auto neighbors = g.getNeighbors(0);
    ASSERT_TRUE(neighbors.empty());
}

TEST_F(GraphTest, MultipleEdges) {
    Graph g(5);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 20);
    g.addEdge(0, 3, 30);

    auto neighbors = g.getNeighbors(0);
    ASSERT_EQ(neighbors.size(), 3);
}

TEST_F(GraphTest, GetEdgesEmpty) {
    Graph g(5);
    auto edges = g.getEdges();
    ASSERT_TRUE(edges.empty());
}

TEST_F(GraphTest, GetEdgesNonEmpty) {
    Graph g(3);
    g.addEdge(0, 1, 10);
    g.addEdge(1, 2, 20);
    g.addEdge(0, 2, 30);

    auto edges = g.getEdges();
    ASSERT_EQ(edges.size(), 3);

    // Verify edge contents
    bool found_edge_0_1 = false;
    bool found_edge_1_2 = false;
    bool found_edge_0_2 = false;

    for (const auto& edge : edges) {
        if (edge.first == 0 && edge.second.first == 1 && edge.second.second == 10) 
            found_edge_0_1 = true;
        if (edge.first == 1 && edge.second.first == 2 && edge.second.second == 20) 
            found_edge_1_2 = true;
        if (edge.first == 0 && edge.second.first == 2 && edge.second.second == 30) 
            found_edge_0_2 = true;
    }

    ASSERT_TRUE(found_edge_0_1);
    ASSERT_TRUE(found_edge_1_2);
    ASSERT_TRUE(found_edge_0_2);
}

TEST_F(GraphTest, GetVerticesCount) {
    Graph g(5);
    ASSERT_EQ(g.getVertices(), 5);
}
