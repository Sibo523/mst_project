#include <gtest/gtest.h>
#include "../Tree/Tree.hpp"
#include <stdexcept>

class TreeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Common setup can be added here
    }

    // Helper function to check if a path exists between two vertices
    bool hasPath(const Tree &tree, int src, int dest, std::vector<bool> &visited)
    {
        if (src == dest)
            return true;

        visited[src] = true;
        const auto &neighbors = tree.getNeighbors(src);

        for (const auto &neighbor : neighbors)
        {
            if (!visited[neighbor.first])
            {
                if (hasPath(tree, neighbor.first, dest, visited))
                {
                    return true;
                }
            }
        }
        return false;
    }
};

TEST_F(TreeTest, ConstructorWithValidVertices)
{
    ASSERT_NO_THROW(Tree t(5));
}

TEST_F(TreeTest, ConstructorWithZeroVertices)
{
    ASSERT_THROW(Tree t(0), std::invalid_argument);
}

TEST_F(TreeTest, AddValidEdge)
{
    Tree t(5);
    ASSERT_NO_THROW(t.addEdge(0, 1, 10));

    auto neighbors0 = t.getNeighbors(0);
    auto neighbors1 = t.getNeighbors(1);

    // Check if edge is added in both directions
    ASSERT_EQ(neighbors0.size(), 1);
    ASSERT_EQ(neighbors1.size(), 1);
    ASSERT_EQ(neighbors0[0].first, 1);
    ASSERT_EQ(neighbors0[0].second, 10);
    ASSERT_EQ(neighbors1[0].first, 0);
    ASSERT_EQ(neighbors1[0].second, 10);
}

TEST_F(TreeTest, AddEdgeOutOfRange)
{
    Tree t(5);
    ASSERT_THROW(t.addEdge(5, 1, 10), std::out_of_range);
    ASSERT_THROW(t.addEdge(0, 5, 10), std::out_of_range);
    ASSERT_THROW(t.addEdge(-1, 1, 10), std::out_of_range);
}

TEST_F(TreeTest, GetNeighborsInvalidVertex)
{
    Tree t(5);
    ASSERT_THROW(t.getNeighbors(-1), std::out_of_range);
    ASSERT_THROW(t.getNeighbors(5), std::out_of_range);
}

TEST_F(TreeTest, GetNeighborsEmptyVertex)
{
    Tree t(5);
    auto neighbors = t.getNeighbors(0);
    ASSERT_TRUE(neighbors.empty());
}

TEST_F(TreeTest, GetEdgesEmpty)
{
    Tree t(5);
    auto edges = t.getEdges();
    ASSERT_TRUE(edges.empty());
}

TEST_F(TreeTest, GetEdgesNonEmpty)
{
    Tree t(3);
    t.addEdge(0, 1, 10);
    t.addEdge(1, 2, 20);

    auto edges = t.getEdges();
    ASSERT_EQ(edges.size(), 2); // Should only count each edge once

    // Verify edge contents
    bool found_edge_0_1 = false;
    bool found_edge_1_2 = false;

    for (const auto &edge : edges)
    {
        if (edge.first == 0 && edge.second.first == 1 && edge.second.second == 10)
            found_edge_0_1 = true;
        if (edge.first == 1 && edge.second.first == 2 && edge.second.second == 20)
            found_edge_1_2 = true;
    }

    ASSERT_TRUE(found_edge_0_1);
    ASSERT_TRUE(found_edge_1_2);
}

TEST_F(TreeTest, PathExistenceBetweenVertices)
{
    Tree t(4);
    t.addEdge(0, 1, 10);
    t.addEdge(1, 2, 20);
    t.addEdge(2, 3, 30);

    std::vector<bool> visited(4, false);
    ASSERT_TRUE(hasPath(t, 0, 3, visited));

    // Reset visited array and check another path
    std::fill(visited.begin(), visited.end(), false);
    ASSERT_TRUE(hasPath(t, 3, 0, visited));
}

TEST_F(TreeTest, GetVerticesCount)
{
    Tree t(5);
    ASSERT_EQ(t.getVertices(), 5);
}

TEST_F(TreeTest, BidirectionalEdges)
{
    Tree t(3);
    t.addEdge(0, 1, 10);

    auto neighbors0 = t.getNeighbors(0);
    auto neighbors1 = t.getNeighbors(1);

    ASSERT_EQ(neighbors0.size(), 1);
    ASSERT_EQ(neighbors1.size(), 1);
    ASSERT_EQ(neighbors0[0].first, 1);
    ASSERT_EQ(neighbors1[0].first, 0);
    ASSERT_EQ(neighbors0[0].second, neighbors1[0].second);
}
