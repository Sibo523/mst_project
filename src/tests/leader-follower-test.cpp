#include <gtest/gtest.h>
#include "../utils/LeaderFollowerThreadPool.hpp"
#include <chrono>
#include <thread>

class LeaderFollowerThreadPoolTest : public ::testing::Test {
protected:
    void SetUp() override {
        pool = std::make_unique<LeaderFollowerThreadPool>(4); // Create pool with 4 threads
    }

    void TearDown() override {
        pool.reset();
    }

    std::unique_ptr<LeaderFollowerThreadPool> pool;
};

TEST_F(LeaderFollowerThreadPoolTest, ProcessEmptyMST) {
    std::vector<std::pair<int, std::pair<int, int>>> emptyMST;
    std::string result = pool->processMST(emptyMST);
    ASSERT_FALSE(result.empty());
    ASSERT_TRUE(result.find("Error: Empty MST provided") != std::string::npos);
}

TEST_F(LeaderFollowerThreadPoolTest, ProcessSimpleMST) {
    std::vector<std::pair<int, std::pair<int, int>>> mst = {
        {2, {0, 1}},
        {3, {1, 2}},
        {4, {2, 3}}
    };

    std::string result = pool->processMST(mst);
    ASSERT_FALSE(result.empty());
    ASSERT_TRUE(result.find("Total Weight:") != std::string::npos);
    ASSERT_TRUE(result.find("Longest Distance:") != std::string::npos);
    ASSERT_TRUE(result.find("Average Distance:") != std::string::npos);
    ASSERT_TRUE(result.find("Shortest Edge:") != std::string::npos);
}

TEST_F(LeaderFollowerThreadPoolTest, ConcurrentTasks) {
    std::atomic<int> counter{0};
    std::vector<std::future<void>> futures;

    for(int i = 0; i < 10; i++) {
        futures.push_back(pool->enqueue([&counter] {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            counter++;
        }));
    }

    for(auto& future : futures) {
        future.get();
    }

    ASSERT_EQ(counter, 10);
}

TEST_F(LeaderFollowerThreadPoolTest, TaskWithReturnValue) {
    auto future = pool->enqueue([] {
        return 42;
    });

    ASSERT_EQ(future.get(), 42);
}

TEST_F(LeaderFollowerThreadPoolTest, MultipleProcessMST) {
    std::vector<std::pair<int, std::pair<int, int>>> mst = {
        {2, {0, 1}},
        {3, {1, 2}},
        {4, {2, 3}}
    };

    std::vector<std::future<std::string>> results;
    for(int i = 0; i < 5; i++) {
        results.push_back(std::async(std::launch::async, [this, &mst] {
            return pool->processMST(mst);
        }));
    }

    for(auto& future : results) {
        std::string result = future.get();
        ASSERT_FALSE(result.empty());
        ASSERT_TRUE(result.find("Total Weight:") != std::string::npos);
    }
}
