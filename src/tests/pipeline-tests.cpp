// #include <gtest/gtest.h>
// #include "utils/Pipeline.hpp"
// #include <string>
// #include <memory>

// class PipelineTest : public ::testing::Test {
// protected:
//     Pipeline pipeline;
// };

// TEST_F(PipelineTest, EmptyPipeline) {
//     auto inputGen = []() -> std::shared_ptr<void> {
//         return std::make_shared<int>(5);
//     };

//     int result = pipeline.process<int>(inputGen);
//     EXPECT_EQ(result, 5);
// }

// TEST_F(PipelineTest, SingleStage) {
//     pipeline.addStage([](std::shared_ptr<void> input) -> std::shared_ptr<void> {
//         int value = *std::static_pointer_cast<int>(input);
//         return std::make_shared<int>(value * 2);
//     });

//     auto inputGen = []() -> std::shared_ptr<void> {
//         return std::make_shared<int>(5);
//     };

//     int result = pipeline.process<int>(inputGen);
//     EXPECT_EQ(result, 10);
// }

// TEST_F(PipelineTest, MultipleStages) {
//     pipeline.addStage([](std::shared_ptr<void> input) -> std::shared_ptr<void> {
//         int value = *std::static_pointer_cast<int>(input);
//         return std::make_shared<int>(value * 2);
//     });

//     pipeline.addStage([](std::shared_ptr<void> input) -> std::shared_ptr<void> {
//         int value = *std::static_pointer_cast<int>(input);
//         return std::make_shared<int>(value + 5);
//     });

//     auto inputGen = []() -> std::shared_ptr<void> {
//         return std::make_shared<int>(5);
//     };

//     int result = pipeline.process<int>(inputGen);
//     EXPECT_EQ(result, 15);
// }

// TEST_F(PipelineTest, DifferentTypes) {
//     pipeline.addStage([](std::shared_ptr<void> input) -> std::shared_ptr<void> {
//         int value = *std::static_pointer_cast<int>(input);
//         return std::make_shared<std::string>(std::to_string(value));
//     });

//     pipeline.addStage([](std::shared_ptr<void> input) -> std::shared_ptr<void> {
//         std::string value = *std::static_pointer_cast<std::string>(input);
//         return std::make_shared<std::string>(value + " processed");
//     });

//     auto inputGen = []() -> std::shared_ptr<void> {
//         return std::make_shared<int>(42);
//     };

//     std::string result = pipeline.process<std::string>(inputGen);
//     EXPECT_EQ(result, "42 processed");
// }
