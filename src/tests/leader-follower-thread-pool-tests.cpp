#include <gtest/gtest.h>
#include "utils/LeaderFollowerThreadPool.hpp"
#include <vector>
#include <atomic>
#include <chrono>

class LeaderFollowerThreadPoolTest : public ::testing::Test
{
protected:
    const size_t NUM_THREADS = 4;
    LeaderFollowerThreadPool pool{NUM_THREADS};
};

TEST_F(LeaderFollowerThreadPoolTest, ExceptionHandling)
{
    auto future = pool.enqueue([]
                               { throw std::runtime_error("Test exception"); });

    EXPECT_THROW(future.get(), std::runtime_error);
}

TEST_F(LeaderFollowerThreadPoolTest, LeaderFollowerBehavior)
{
    std::atomic<int> leaderCount(0);
    std::vector<std::future<void>> futures;

    for (int i = 0; i < 100; ++i)
    {
        futures.emplace_back(
            pool.enqueue([&leaderCount]
                         {
                if (leaderCount++ == 0) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                leaderCount--; }));
    }

    for (auto &future : futures)
    {
        future.wait();
    }

    EXPECT_EQ(leaderCount, 0);
}
