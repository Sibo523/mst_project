#include <gtest/gtest.h>
#include "factory/MSTFactory.hpp"
#include "mst/BoruvkaMST.hpp"
#include "mst/PrimMST.hpp"
#include "mst/KruskalMST.hpp"
#include "mst/TarjanMST.hpp"
#include "mst/IntegerMST.hpp"
#include <memory>
#include <typeinfo>

class MSTFactoryTest : public ::testing::Test {};

TEST_F(MSTFactoryTest, CreateBoruvkaMST) {
    auto mst = MSTFactory::createMSTAlgorithm("Boruvka");
    EXPECT_NE(mst, nullptr);
    EXPECT_EQ(typeid(*mst), typeid(BoruvkaMST));
}

TEST_F(MSTFactoryTest, CreatePrimMST) {
    auto mst = MSTFactory::createMSTAlgorithm("Prim");
    EXPECT_NE(mst, nullptr);
    EXPECT_EQ(typeid(*mst), typeid(PrimMST));
}

TEST_F(MSTFactoryTest, CreateKruskalMST) {
    auto mst = MSTFactory::createMSTAlgorithm("Kruskal");
    EXPECT_NE(mst, nullptr);
    EXPECT_EQ(typeid(*mst), typeid(KruskalMST));
}

TEST_F(MSTFactoryTest, CreateTarjanMST) {
    auto mst = MSTFactory::createMSTAlgorithm("Tarjan");
    EXPECT_NE(mst, nullptr);
    EXPECT_EQ(typeid(*mst), typeid(TarjanMST));
}

TEST_F(MSTFactoryTest, CreateIntegerMST) {
    auto mst = MSTFactory::createMSTAlgorithm("Integer");
    EXPECT_NE(mst, nullptr);
    EXPECT_EQ(typeid(*mst), typeid(IntegerMST));
}

TEST_F(MSTFactoryTest, CreateUnknownMST) {
    EXPECT_THROW(MSTFactory::createMSTAlgorithm("UnknownAlgorithm"), std::invalid_argument);
}
