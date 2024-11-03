#include <gtest/gtest.h>
#include "../analysis/MSTAnalysis.hpp"
#include <vector>
#include <limits>

class MSTAnalysisTest : public ::testing::Test {
protected:
    // Sample MST data for testing
    std::vector<std::pair<int, std::pair<int, int>>> sampleMST;
    std::vector<std::pair<int, std::pair<int, int>>> emptyMST;
    std::vector<std::pair<int, std::pair<int, int>>> linearMST;

    void SetUp() override {
        // Create a sample MST with known properties
        sampleMST = {
            {2, {0, 1}},
            {3, {1, 2}},
            {4, {2, 3}}
        };

        // Create a linear MST for predictable distance calculations
        linearMST = {
            {1, {0, 1}},
            {1, {1, 2}},
            {1, {2, 3}}
        };
    }
};

TEST_F(MSTAnalysisTest, CalculateTotalWeight) {
    auto result = MSTAnalysis::calculateTotalWeight(sampleMST);
    ASSERT_EQ(result.first, 0);  // Index for total weight
    ASSERT_EQ(result.second, 9); // 2 + 3 + 4 = 9
}

TEST_F(MSTAnalysisTest, CalculateTotalWeightEmpty) {
    auto result = MSTAnalysis::calculateTotalWeight(emptyMST);
    ASSERT_EQ(result.first, 0);
    ASSERT_EQ(result.second, 0);
}

TEST_F(MSTAnalysisTest, FindLongestDistance) {
    auto result = MSTAnalysis::findLongestDistance(sampleMST);
    ASSERT_EQ(result.first, 1);  // Index for longest distance
    ASSERT_EQ(result.second, 9); // Longest path from vertex 0 to 3: 2 + 3 + 4 = 9
}

TEST_F(MSTAnalysisTest, FindLongestDistanceLinear) {
    auto result = MSTAnalysis::findLongestDistance(linearMST);
    ASSERT_EQ(result.first, 1);
    ASSERT_EQ(result.second, 3); // In linear MST with weight 1, longest distance is 3
}

TEST_F(MSTAnalysisTest, CalculateAverageDistance) {
    auto result = MSTAnalysis::calculateAverageDistance(linearMST);
    ASSERT_EQ(result.first, 2);  // Index for average distance
    
    // In a linear graph with 4 vertices and weight 1:
    // Distances: 1,2,3,1,2,1
    // Average = (1+2+3+1+2+1)/6 = 1.67
    ASSERT_NEAR(result.second, 1.67, 0.01);
}

TEST_F(MSTAnalysisTest, FindShortestMSTEdge) {
    auto result = MSTAnalysis::findShortestMSTEdge(sampleMST);
    ASSERT_EQ(result.first, 3);  // Index for shortest edge
    ASSERT_EQ(result.second, 2); // Minimum weight is 2
}

TEST_F(MSTAnalysisTest, FindShortestMSTEdgeEmpty) {
    auto result = MSTAnalysis::findShortestMSTEdge(emptyMST);
    ASSERT_EQ(result.first, 3);
    ASSERT_EQ(result.second, 0);
}

TEST_F(MSTAnalysisTest, GetPipelineFunctions) {
    auto functions = MSTAnalysis::getPipelineFunctions();
    ASSERT_EQ(functions.size(), 4);  // Should return 4 analysis functions
    
    // Test each function with sample data
    for(size_t i = 0; i < functions.size(); i++) {
        auto result = functions[i](sampleMST);
        ASSERT_EQ(result.first, i);  // Each function should return its index
        ASSERT_GE(result.second, 0); // Result should be non-negative
    }
}

TEST_F(MSTAnalysisTest, NegativeWeights) {
    std::vector<std::pair<int, std::pair<int, int>>> negativeWeightMST = {
        {-2, {0, 1}},
        {3, {1, 2}},
        {4, {2, 3}}
    };

    auto result = MSTAnalysis::calculateTotalWeight(negativeWeightMST);
    ASSERT_EQ(result.first, 0);
    ASSERT_EQ(result.second, 7); // Should ignore negative weight
}

TEST_F(MSTAnalysisTest, LargeGraph) {
    std::vector<std::pair<int, std::pair<int, int>>> largeMST;
    // Create a larger MST to test performance
    for(int i = 0; i < 100; i++) {
        largeMST.push_back({1, {i, i+1}});
    }

    ASSERT_NO_THROW({
        MSTAnalysis::calculateTotalWeight(largeMST);
        MSTAnalysis::findLongestDistance(largeMST);
        MSTAnalysis::calculateAverageDistance(largeMST);
        MSTAnalysis::findShortestMSTEdge(largeMST);
    });
}
