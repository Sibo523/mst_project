#include <gtest/gtest.h>
#include "graph/Graph.hpp"
#include <stdexcept>
#include <algorithm>

class GraphTest : public ::testing::Test
{
protected:
    Graph graph{5}; // Create a graph with 5 vertices
};

TEST_F(GraphTest, AddEdge)
{
    EXPECT_NO_THROW(graph.addEdge(0, 1, 10));
    EXPECT_NO_THROW(graph.addEdge(1, 2, 20));
    EXPECT_NO_THROW(graph.addEdge(2, 3, 30));
    EXPECT_NO_THROW(graph.addEdge(3, 4, 40));
    EXPECT_NO_THROW(graph.addEdge(4, 0, 50));
}

TEST_F(GraphTest, AddEdgeOutOfRange)
{
    EXPECT_THROW(graph.addEdge(-1, 1, 10), std::out_of_range);
    EXPECT_THROW(graph.addEdge(0, 5, 10), std::out_of_range);
    EXPECT_THROW(graph.addEdge(5, 0, 10), std::out_of_range);
}

TEST_F(GraphTest, GetNeighbors)
{
    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 20);

    const auto &neighbors = graph.getNeighbors(0);
    EXPECT_EQ(neighbors.size(), 2);
    EXPECT_EQ(neighbors[0], std::make_pair(1, 10));
    EXPECT_EQ(neighbors[1], std::make_pair(2, 20));
}

TEST_F(GraphTest, GetNeighborsOutOfRange)
{
    EXPECT_THROW(graph.getNeighbors(-1), std::out_of_range);
    EXPECT_THROW(graph.getNeighbors(5), std::out_of_range);
}

TEST_F(GraphTest, GetEdges)
{
    graph.addEdge(0, 1, 10);
    graph.addEdge(1, 2, 20);
    graph.addEdge(2, 3, 30);

    auto edges = graph.getEdges();
    EXPECT_EQ(edges.size(), 3);
    EXPECT_TRUE(std::find(edges.begin(), edges.end(), std::make_pair(0, std::make_pair(1, 10))) != edges.end());
    EXPECT_TRUE(std::find(edges.begin(), edges.end(), std::make_pair(1, std::make_pair(2, 20))) != edges.end());
    EXPECT_TRUE(std::find(edges.begin(), edges.end(), std::make_pair(2, std::make_pair(3, 30))) != edges.end());
}

TEST_F(GraphTest, GetVertices)
{
    EXPECT_EQ(graph.getVertices(), 5);
}
