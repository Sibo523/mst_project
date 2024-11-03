#include <gtest/gtest.h>
#include "../utils/Pipeline.hpp"
#include <vector>
#include <chrono>
#include <thread>

class PipelineTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a vector of functions for testing
        functions = {
            [](const std::vector<std::pair<int, std::pair<int, int>>>& data) {
                return std::make_pair(0, 10.0); // Total weight
            },
            [](const std::vector<std::pair<int, std::pair<int, int>>>& data) {
                return std::make_pair(1, 20.0); // Longest distance
            },
            [](const std::vector<std::pair<int, std::pair<int, int>>>& data) {
                return std::make_pair(2, 15.0); // Average distance
            },
            [](const std::vector<std::pair<int, std::pair<int, int>>>& data) {
                return std::make_pair(3, 5.0);  // Shortest edge
            }
        };
        
        pipeline = std::make_unique<Pipeline>(functions);
    }

    void TearDown() override {
        pipeline->stop();
    }

    std::vector<std::function<std::pair<int, double>(const std::vector<std::pair<int, std::pair<int, int>>>&)>> functions;
    std::unique_ptr<Pipeline> pipeline;
};

TEST_F(PipelineTest, StartAndStop) {
    pipeline->start();
    ASSERT_NO_THROW(pipeline->stop());
}

TEST_F(PipelineTest, BasicExecution) {
    pipeline->start();
    
    std::vector<std::pair<int, std::pair<int, int>>> testData = {
        {2, {0, 1}},
        {3, {1, 2}},
        {4, {2, 3}}
    };

    pipeline->execute(testData);
    std::string result = pipeline->getResult();

    ASSERT_FALSE(result.empty());
    ASSERT_TRUE(result.find("Total weight: 10") != std::string::npos);
    ASSERT_TRUE(result.find("Longest distance: 20") != std::string::npos);
    ASSERT_TRUE(result.find("Average distance: 15") != std::string::npos);
    ASSERT_TRUE(result.find("Shortest MST edge: 5") != std::string::npos);
}

// TEST_F(PipelineTest, EmptyInput) {
//     pipeline->start();
//     std::vector<std::pair<int, std::pair<int, int>>> emptyData;
    
//     pipeline->execute(emptyData);
//     std::string result = pipeline->getResult();
    
//     ASSERT_FALSE(result.empty());
// }

TEST_F(PipelineTest, MultipleExecutions) {
    pipeline->start();
    
    std::vector<std::pair<int, std::pair<int, int>>> testData = {
        {2, {0, 1}},
        {3, {1, 2}}
    };

    for(int i = 0; i < 3; i++) {
        pipeline->execute(testData);
        std::string result = pipeline->getResult();
        ASSERT_FALSE(result.empty());
    }
}

// TEST_F(PipelineTest, ExecuteWithoutStart) {
//     std::vector<std::pair<int, std::pair<int, int>>> testData = {
//         {2, {0, 1}},
//         {3, {1, 2}}
//     };

//     pipeline->execute(testData);
//     std::string result = pipeline->getResult();
//     ASSERT_FALSE(result.empty());
// }
