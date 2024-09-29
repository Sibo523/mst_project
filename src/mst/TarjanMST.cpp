#include "TarjanMST.hpp"
#include <algorithm>
#include <cmath>
#include <stack>
std::vector<std::pair<int, std::pair<int, int>>> TarjanMST::findMST(const Graph &graph)
{
    int V = graph.getVertices();
    if (V == 0)
    {
        std::cout << "Warning: Empty graph passed to TarjanMST::findMST" << std::endl;
        return {};
    }

    std::vector<Edge> edges;
    std::vector<std::pair<int, std::pair<int, int>>> mst;
    std::vector<int> parent(V), rank(V, 0);

    // Collect all edges from the graph
    for (int i = 0; i < V; i++)
    {
        for (const auto &[dest, weight] : graph.getNeighbors(i))
        {
            if (i < dest) // Avoid duplicate edges
            {
                edges.emplace_back(i, dest, weight);
            }
        }
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return a.weight < b.weight;
    });

    // Initialize disjoint set
    for (int i = 0; i < V; i++)
    {
        parent[i] = i;
    }

    // Kruskal's algorithm
    for (const auto &edge : edges)
    {
        int x = find(parent, edge.src);
        int y = find(parent, edge.dest);

        if (x != y)
        {
            mst.push_back({edge.weight, {edge.src, edge.dest}});
            unionSet(parent, rank, x, y);
        }

        if (mst.size() == V - 1)
        {
            break; // MST is complete
        }
    }

    return mst;
}
void TarjanMST::iterativeMST(const Graph &graph, std::vector<std::pair<int, std::pair<int, int>>> &mst,
                             std::vector<int> &parent, std::vector<int> &rank,
                             std::vector<Edge> &edges)
{
    std::stack<std::tuple<std::vector<Edge>, int>> stack;
    stack.push({edges, edges.size()});

    while (!stack.empty())
    {
        auto [edges, m] = stack.top();
        stack.pop();

        if (m <= 1)
        {
            if (m == 1)
            {
                mst.push_back({edges[0].weight, {edges[0].src, edges[0].dest}});
            }
            continue;
        }

        int k = std::ceil(std::pow(m, 0.75));
        std::vector<Edge> F = sampleEdges(edges, k);

        // Push the sampled edges onto the stack for recursive processing
        stack.push({F, F.size()});

        // Process remaining edges
        std::vector<Edge> remaining;
        for (const Edge &e : edges)
        {
            int x = find(parent, e.src);
            int y = find(parent, e.dest);
            if (x != y)
            {
                remaining.push_back(e);
            }
        }

        // Final processing
        for (const Edge &e : remaining)
        {
            int x = find(parent, e.src);
            int y = find(parent, e.dest);
            if (x != y)
            {
                mst.push_back({e.weight, {e.src, e.dest}});
                unionSet(parent, rank, x, y);
            }
        }

        // Push the remaining edges onto the stack for recursive processing
        stack.push({remaining, remaining.size()});
    }
}

int TarjanMST::find(std::vector<int> &parent, int i)
{
    if (parent[i] != i)
    {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void TarjanMST::unionSet(std::vector<int> &parent, std::vector<int> &rank, int x, int y)
{
    int xroot = find(parent, x);
    int yroot = find(parent, y);

    if (rank[xroot] < rank[yroot])
    {
        parent[xroot] = yroot;
    }
    else if (rank[xroot] > rank[yroot])
    {
        parent[yroot] = xroot;
    }
    else
    {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

std::vector<TarjanMST::Edge> TarjanMST::sampleEdges(const std::vector<Edge> &edges, int k)
{
    std::vector<Edge> sampled = edges;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(sampled.begin(), sampled.end(), g);
    sampled.resize(std::min(k, static_cast<int>(sampled.size())));
    return sampled;
}