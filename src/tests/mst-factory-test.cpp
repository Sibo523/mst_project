#include <gtest/gtest.h>
#include "../factory/MSTFactory.hpp"
#include "../mst/BoruvkaMST.hpp"
#include "../mst/PrimMST.hpp"
#include "../mst/KruskalMST.hpp"
#include "../mst/TarjanMST.hpp"
#include "../mst/IntegerMST.hpp"
#include <stdexcept>

class MSTFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(MSTFactoryTest, CreateBoruvka) {
    auto algorithm = MSTFactory::createMSTAlgorithm("Boruvka");
    ASSERT_NE(algorithm, nullptr);
    ASSERT_NE(dynamic_cast<BoruvkaMST*>(algorithm.get()), nullptr);
}

TEST_F(MSTFactoryTest, CreatePrim) {
    auto algorithm = MSTFactory::createMSTAlgorithm("Prim");
    ASSERT_NE(algorithm, nullptr);
    ASSERT_NE(dynamic_cast<PrimMST*>(algorithm.get()), nullptr);
}

TEST_F(MSTFactoryTest, CreateKruskal) {
    auto algorithm = MSTFactory::createMSTAlgorithm("Kruskal");
    ASSERT_NE(algorithm, nullptr);
    ASSERT_NE(dynamic_cast<KruskalMST*>(algorithm.get()), nullptr);
}

TEST_F(MSTFactoryTest, CreateTarjan) {
    auto algorithm = MSTFactory::createMSTAlgorithm("Tarjan");
    ASSERT_NE(algorithm, nullptr);
    ASSERT_NE(dynamic_cast<TarjanMST*>(algorithm.get()), nullptr);
}

TEST_F(MSTFactoryTest, CreateInteger) {
    auto algorithm = MSTFactory::createMSTAlgorithm("Integer");
    ASSERT_NE(algorithm, nullptr);
    ASSERT_NE(dynamic_cast<IntegerMST*>(algorithm.get()), nullptr);
}

TEST_F(MSTFactoryTest, InvalidAlgorithmName) {
    ASSERT_THROW(MSTFactory::createMSTAlgorithm("InvalidAlgorithm"), std::invalid_argument);
}

TEST_F(MSTFactoryTest, EmptyAlgorithmName) {
    ASSERT_THROW(MSTFactory::createMSTAlgorithm(""), std::invalid_argument);
}
