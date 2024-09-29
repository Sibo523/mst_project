#include <gtest/gtest.h>
#include "../Tree/Tree.hpp"
#include <stdexcept>
#include <algorithm>

class TreeTest : public ::testing::Test
{
protected:
    Tree tree{5}; // Create a tree with 5 vertices
};

TEST_F(TreeTest, AddEdge)
{
    EXPECT_NO_THROW(tree.addEdge(0, 1, 10));
    EXPECT_NO_THROW(tree.addEdge(1, 2, 20));
    EXPECT_NO_THROW(tree.addEdge(2, 3, 30));
    EXPECT_NO_THROW(tree.addEdge(3, 4, 40));
}

TEST_F(TreeTest, AddEdgeOutOfRange)
{
    EXPECT_THROW(tree.addEdge(-1, 1, 10), std::out_of_range);
    EXPECT_THROW(tree.addEdge(0, 5, 10), std::out_of_range);
    EXPECT_THROW(tree.addEdge(5, 0, 10), std::out_of_range);
}

TEST_F(TreeTest, GetNeighbors)
{
    tree.addEdge(0, 1, 10);
    tree.addEdge(0, 2, 20);

    const auto &neighbors = tree.getNeighbors(0);
    EXPECT_EQ(neighbors.size(), 2);
    EXPECT_TRUE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(1, 10)) != neighbors.end());
    EXPECT_TRUE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(2, 20)) != neighbors.end());
}

TEST_F(TreeTest, GetNeighborsOutOfRange)
{
    EXPECT_THROW(tree.getNeighbors(-1), std::out_of_range);
    EXPECT_THROW(tree.getNeighbors(5), std::out_of_range);
}

TEST_F(TreeTest, GetEdges)
{
    tree.addEdge(0, 1, 10);
    tree.addEdge(1, 2, 20);
    tree.addEdge(2, 3, 30);
    tree.addEdge(3, 4, 40);

    auto edges = tree.getEdges();
    EXPECT_EQ(edges.size(), 4);
    EXPECT_TRUE(std::find(edges.begin(), edges.end(), std::make_pair(0, std::make_pair(1, 10))) != edges.end());
    EXPECT_TRUE(std::find(edges.begin(), edges.end(), std::make_pair(1, std::make_pair(2, 20))) != edges.end());
    EXPECT_TRUE(std::find(edges.begin(), edges.end(), std::make_pair(2, std::make_pair(3, 30))) != edges.end());
    EXPECT_TRUE(std::find(edges.begin(), edges.end(), std::make_pair(3, std::make_pair(4, 40))) != edges.end());
}

TEST_F(TreeTest, GetVertices)
{
    EXPECT_EQ(tree.getVertices(), 5);
}

TEST_F(TreeTest, BidirectionalEdges)
{
    tree.addEdge(0, 1, 10);

    const auto &neighbors0 = tree.getNeighbors(0);
    const auto &neighbors1 = tree.getNeighbors(1);

    EXPECT_EQ(neighbors0.size(), 1);
    EXPECT_EQ(neighbors1.size(), 1);
    EXPECT_TRUE(std::find(neighbors0.begin(), neighbors0.end(), std::make_pair(1, 10)) != neighbors0.end());
    EXPECT_TRUE(std::find(neighbors1.begin(), neighbors1.end(), std::make_pair(0, 10)) != neighbors1.end());
}
